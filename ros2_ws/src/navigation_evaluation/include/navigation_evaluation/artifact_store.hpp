#pragma once

#include <json/json.h>

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace navigation_evaluation {

struct RunMetadata
{
  std::string run_id;
  std::optional<std::string> batch_id;
  std::string scenario_id;
  std::string configuration_hash;
  std::string profile_id;
  double created_at{0.0};
  std::string created_at_clock_domain;
  std::uint64_t clock_segment{0};
  Json::Value environment{Json::objectValue};
};

struct RunEvent
{
  std::string run_id;
  std::string event_id;
  std::uint64_t sequence{0};
  std::string event_type;
  double observed_at{0.0};
  std::string clock_domain;
  std::uint64_t clock_segment{0};
  std::string source;
  Json::Value payload{Json::objectValue};
};

class ArtifactStore
{
public:
  ArtifactStore(std::filesystem::path root, std::string batch_id, std::string run_id);

  void create_partial_run(const RunMetadata &metadata);
  void append_event(const RunEvent &event);
  void complete_run(
    const std::string &terminal_status,
    std::optional<std::string> failure_class,
    std::optional<std::int64_t> duration_ms);

  [[nodiscard]] const std::filesystem::path &run_path() const noexcept { return run_path_; }
  [[nodiscard]] const std::filesystem::path &events_path() const noexcept { return events_path_; }

  static std::vector<Json::Value> read_complete_events(const std::filesystem::path &events_path);

private:
  void write_run_record(const Json::Value &record) const;

  std::filesystem::path run_path_;
  std::filesystem::path events_path_;
  Json::Value run_record_{Json::objectValue};
  std::unordered_set<std::string> event_ids_;
};

}  // namespace navigation_evaluation