#include "navigation_evaluation/scenario.hpp"

#include <yaml-cpp/yaml.h>

#include <regex>
#include <sstream>

namespace navigation_evaluation {
namespace {

constexpr int kSupportedSchemaMajor = 1;

template<typename T>
T required_value(const YAML::Node &node, const char *key, const std::filesystem::path &path)
{
  const auto value = node[key];
  if (!value) {
    throw ScenarioError("Missing '" + std::string(key) + "' in " + path.string());
  }

  try {
    return value.as<T>();
  } catch (const YAML::Exception &error) {
    throw ScenarioError(
      "Invalid '" + std::string(key) + "' in " + path.string() + ": " + error.what());
  }
}

void validate_pose(const Pose2D &pose, const char *name, const std::filesystem::path &path)
{
  if (pose.frame_id.empty()) {
    throw ScenarioError("'" + std::string(name) + ".frame_id' must not be empty in " + path.string());
  }
}

}  // namespace

ResetPolicy parse_reset_policy(const std::string &value)
{
  if (value == "world") {
    return ResetPolicy::World;
  }
  if (value == "robot") {
    return ResetPolicy::Robot;
  }
  if (value == "navigation") {
    return ResetPolicy::Navigation;
  }
  throw ScenarioError("Unsupported reset policy: " + value);
}

FaultMode parse_fault_mode(const std::string &value)
{
  if (value == "delay") {
    return FaultMode::Delay;
  }
  if (value == "drop") {
    return FaultMode::Drop;
  }
  throw ScenarioError("Unsupported fault mode: " + value);
}

Scenario load_scenario(const std::filesystem::path &path)
{
  if (path.extension() != ".yaml") {
    throw ScenarioError("Scenario file must use the .yaml extension: " + path.string());
  }
  static const std::regex filename_pattern("^[a-z0-9]+(_[a-z0-9]+)*\\.yaml$");
  if (!std::regex_match(path.filename().string(), filename_pattern)) {
    throw ScenarioError("Scenario filename must use lowercase snake_case: " + path.filename().string());
  }

  YAML::Node root;
  try {
    root = YAML::LoadFile(path.string());
  } catch (const YAML::Exception &error) {
    throw ScenarioError("Unable to parse " + path.string() + ": " + error.what());
  }

  Scenario scenario;
  scenario.schema_version = required_value<std::string>(root, "schema_version", path);
  const auto separator = scenario.schema_version.find('.');
  if (separator == std::string::npos) {
    throw ScenarioError("schema_version must be semantic version text in " + path.string());
  }
  try {
    if (std::stoi(scenario.schema_version.substr(0, separator)) != kSupportedSchemaMajor) {
      throw ScenarioError("Unsupported scenario schema major version: " + scenario.schema_version);
    }
  } catch (const std::invalid_argument &) {
    throw ScenarioError("schema_version must start with a numeric major version in " + path.string());
  }

  scenario.scenario_id = required_value<std::string>(root, "scenario_id", path);
  if (!std::regex_match(scenario.scenario_id, std::regex("^[a-z0-9]+(_[a-z0-9]+)*$"))) {
    throw ScenarioError("scenario_id must use lowercase snake_case in " + path.string());
  }
  scenario.description = required_value<std::string>(root, "description", path);

  const auto parse_pose = [&](const char *name) {
    const auto node = root[name];
    if (!node || !node.IsMap()) {
      throw ScenarioError("'" + std::string(name) + "' must be a map in " + path.string());
    }
    Pose2D pose;
    pose.frame_id = required_value<std::string>(node, "frame_id", path);
    pose.x = required_value<double>(node, "x", path);
    pose.y = required_value<double>(node, "y", path);
    pose.yaw = required_value<double>(node, "yaw", path);
    validate_pose(pose, name, path);
    return pose;
  };

  scenario.start_pose = parse_pose("start_pose");
  scenario.goal_pose = parse_pose("goal_pose");
  scenario.readiness_timeout_s = required_value<double>(root, "readiness_timeout_s", path);
  scenario.task_timeout_s = required_value<double>(root, "task_timeout_s", path);
  if (scenario.readiness_timeout_s <= 0.0 || scenario.task_timeout_s <= 0.0) {
    throw ScenarioError("Scenario timeouts must be positive in " + path.string());
  }
  scenario.reset_policy = parse_reset_policy(required_value<std::string>(root, "reset_policy", path));

  if (root["seed"] && !root["seed"].IsNull()) {
    scenario.seed = root["seed"].as<int>();
  }

  const auto faults = root["faults"];
  if (!faults || !faults.IsSequence()) {
    throw ScenarioError("'faults' must be a sequence in " + path.string());
  }
  for (const auto &node : faults) {
    FaultSchedule fault;
    fault.fault_id = required_value<std::string>(node, "fault_id", path);
    fault.target_stream = required_value<std::string>(node, "target_stream", path);
    if (fault.target_stream != "lidar") {
      throw ScenarioError("Only the lidar fault target is supported in " + path.string());
    }
    fault.mode = parse_fault_mode(required_value<std::string>(node, "mode", path));
    fault.start_offset_s = required_value<double>(node, "start_offset_s", path);
    fault.duration_s = required_value<double>(node, "duration_s", path);
    if (fault.start_offset_s < 0.0 || fault.duration_s <= 0.0) {
      throw ScenarioError("Fault offsets and durations must be non-negative/positive in " + path.string());
    }
    if (fault.mode == FaultMode::Delay) {
      fault.delay_ms = required_value<double>(node, "delay_ms", path);
      if (*fault.delay_ms < 0.0) {
        throw ScenarioError("A delay fault requires a non-negative delay_ms in " + path.string());
      }
    }
    scenario.faults.push_back(std::move(fault));
  }

  return scenario;
}

}  // namespace navigation_evaluation