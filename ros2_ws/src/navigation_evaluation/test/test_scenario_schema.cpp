#include "navigation_evaluation/scenario.hpp"

#include <gtest/gtest.h>

#include <filesystem>

namespace {

std::filesystem::path fixture_path(const char *name)
{
  return std::filesystem::path(NAVIGATION_EVALUATION_TEST_FIXTURE_DIR) / name;
}

}  // namespace

TEST(ScenarioSchema, LoadsHealthyScenario)
{
  const auto scenario = navigation_evaluation::load_scenario(fixture_path("healthy.yaml"));

  EXPECT_EQ(scenario.schema_version, "1.0.0");
  EXPECT_EQ(scenario.scenario_id, "healthy");
  EXPECT_EQ(scenario.start_pose.frame_id, "map");
  EXPECT_DOUBLE_EQ(scenario.goal_pose.x, 2.5);
  EXPECT_DOUBLE_EQ(scenario.goal_pose.yaw, 1.5707963267948966);
  EXPECT_EQ(scenario.reset_policy, navigation_evaluation::ResetPolicy::World);
  EXPECT_TRUE(scenario.faults.empty());
}

TEST(ScenarioSchema, RejectsInvalidFaultConfiguration)
{
  EXPECT_THROW(
    navigation_evaluation::load_scenario(fixture_path("invalid_fault.yaml")),
    navigation_evaluation::ScenarioError);
}

TEST(ScenarioSchema, RejectsUnsupportedResetPolicy)
{
  EXPECT_THROW(
    navigation_evaluation::parse_reset_policy("everything"),
    navigation_evaluation::ScenarioError);
}

TEST(ScenarioSchema, RejectsNonSnakeCaseFilename)
{
  EXPECT_THROW(
    navigation_evaluation::load_scenario("BadName.yaml"),
    navigation_evaluation::ScenarioError);
}