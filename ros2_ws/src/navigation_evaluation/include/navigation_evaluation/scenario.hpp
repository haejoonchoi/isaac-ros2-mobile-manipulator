#pragma once

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace navigation_evaluation {

class ScenarioError : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};

struct Pose2D
{
  std::string frame_id;
  double x{0.0};
  double y{0.0};
  double yaw{0.0};
};

enum class ResetPolicy
{
  World,
  Robot,
  Navigation
};

enum class FaultMode
{
  Delay,
  Drop
};

struct FaultSchedule
{
  std::string fault_id;
  std::string target_stream;
  FaultMode mode{FaultMode::Drop};
  double start_offset_s{0.0};
  double duration_s{0.0};
  std::optional<double> delay_ms;
};

struct Scenario
{
  std::string schema_version;
  std::string scenario_id;
  std::string description;
  Pose2D start_pose;
  Pose2D goal_pose;
  double readiness_timeout_s{0.0};
  double task_timeout_s{0.0};
  ResetPolicy reset_policy{ResetPolicy::World};
  std::vector<FaultSchedule> faults;
  std::optional<int> seed;
};

ResetPolicy parse_reset_policy(const std::string &value);
FaultMode parse_fault_mode(const std::string &value);
Scenario load_scenario(const std::filesystem::path &path);

}  // namespace navigation_evaluation