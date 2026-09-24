#include "navigation_evaluation/artifact_store.hpp"

#include <gtest/gtest.h>

#include <fstream>

TEST(ArtifactStore, WritesPartialAndCompleteRunRecords)
{
  const auto root = std::filesystem::temp_directory_path() / "navigation_evaluation_artifacts";
  std::filesystem::remove_all(root);
  navigation_evaluation::ArtifactStore store(root, "batch-1", "run-1");

  navigation_evaluation::RunMetadata metadata;
  metadata.run_id = "run-1";
  metadata.batch_id = "batch-1";
  metadata.scenario_id = "healthy";
  metadata.configuration_hash = "abc";
  store.create_partial_run(metadata);

  std::ifstream partial(store.run_path());
  Json::Value partial_record;
  partial >> partial_record;
  EXPECT_EQ(partial_record["artifact_state"].asString(), "partial");

  navigation_evaluation::RunEvent event;
  event.run_id = "run-1";
  event.sequence = 1;
  event.event_type = "run.lifecycle";
  event.observed_at = 12.5;
  event.clock_domain = "steady";
  event.source = "test";
  store.append_event(event);
  store.complete_run("succeeded", std::nullopt, 1250);

  std::ifstream complete(store.run_path());
  Json::Value complete_record;
  complete >> complete_record;
  EXPECT_EQ(complete_record["artifact_state"].asString(), "complete");
  EXPECT_EQ(complete_record["terminal_status"].asString(), "succeeded");
  EXPECT_EQ(complete_record["duration_ms"].asInt64(), 1250);
  EXPECT_EQ(store.read_complete_events(store.events_path()).size(), 1U);

  std::filesystem::remove_all(root);
}

TEST(ArtifactStore, IgnoresUnterminatedFinalEvent)
{
  const auto path = std::filesystem::temp_directory_path() / "navigation_events.jsonl";
  {
    std::ofstream output(path);
    output << "{\"sequence\":1}\n{\"sequence\":2}";
  }

  const auto events = navigation_evaluation::ArtifactStore::read_complete_events(path);
  EXPECT_EQ(events.size(), 1U);
  EXPECT_EQ(events.front()["sequence"].asUInt(), 1U);
  std::filesystem::remove(path);
}