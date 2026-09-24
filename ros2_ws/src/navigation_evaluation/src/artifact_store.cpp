#include "navigation_evaluation/artifact_store.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace navigation_evaluation {
namespace {

std::string render_json(const Json::Value &value)
{
  Json::StreamWriterBuilder builder;
  builder["indentation"] = "";
  return Json::writeString(builder, value);
}

void write_atomically(const std::filesystem::path &path, const std::string &contents)
{
  const auto temporary_path = path.string() + ".tmp";
  {
    std::ofstream output(temporary_path, std::ios::binary | std::ios::trunc);
    if (!output) {
      throw std::runtime_error("Unable to write artifact: " + temporary_path);
    }
    output << contents;
    output.flush();
    if (!output) {
      throw std::runtime_error("Unable to flush artifact: " + temporary_path);
    }
  }
  std::error_code error;
  std::filesystem::rename(temporary_path, path, error);
  if (error) {
    std::filesystem::remove(path, error);
    error.clear();
    std::filesystem::rename(temporary_path, path, error);
  }
  if (error) {
    throw std::runtime_error("Unable to replace artifact: " + path.string());
  }
}

}  // namespace

ArtifactStore::ArtifactStore(std::filesystem::path root, std::string batch_id, std::string run_id)
{
  run_path_ = std::move(root) / batch_id / "runs" / run_id / "run.json";
  events_path_ = run_path_.parent_path() / "events.jsonl";
}

void ArtifactStore::create_partial_run(const RunMetadata &metadata)
{
  if (metadata.run_id.empty() || metadata.scenario_id.empty()) {
    throw std::invalid_argument("run_id and scenario_id are required");
  }
  std::filesystem::create_directories(run_path_.parent_path());
  run_record_["schema_version"] = "1.0.0";
  run_record_["run_id"] = metadata.run_id;
  if (metadata.batch_id) {
    run_record_["batch_id"] = *metadata.batch_id;
  } else {
    run_record_["batch_id"] = Json::nullValue;
  }
  run_record_["scenario_id"] = metadata.scenario_id;
  run_record_["configuration_hash"] = metadata.configuration_hash;
  run_record_["environment"] = metadata.environment;
  run_record_["artifact_state"] = "partial";
  run_record_["events_file"] = "events.jsonl";
  write_run_record(run_record_);
}

void ArtifactStore::append_event(const RunEvent &event)
{
  if (event.run_id.empty() || event.event_type.empty() || event.clock_domain.empty()) {
    throw std::invalid_argument("run event requires identity, type, and clock domain");
  }
  std::ofstream output(events_path_, std::ios::app);
  if (!output) {
    throw std::runtime_error("Unable to open event artifact: " + events_path_.string());
  }
  Json::Value value(Json::objectValue);
  value["run_id"] = event.run_id;
  value["sequence"] = Json::UInt64(event.sequence);
  value["event_type"] = event.event_type;
  value["observed_at"]["value"] = event.observed_at;
  value["observed_at"]["clock_domain"] = event.clock_domain;
  value["source"] = event.source;
  value["payload"] = event.payload;
  output << render_json(value) << '\n';
}

void ArtifactStore::complete_run(
  const std::string &terminal_status,
  std::optional<std::string> failure_class,
  std::optional<std::int64_t> duration_ms)
{
  if (terminal_status != "succeeded" && terminal_status != "failed" &&
      terminal_status != "canceled" && terminal_status != "interrupted") {
    throw std::invalid_argument("Unsupported terminal status: " + terminal_status);
  }
  run_record_["terminal_status"] = terminal_status;
  if (failure_class) {
    run_record_["failure_class"] = *failure_class;
  } else {
    run_record_["failure_class"] = Json::nullValue;
  }
  if (duration_ms) {
    run_record_["duration_ms"] = Json::Int64(*duration_ms);
  } else {
    run_record_["duration_ms"] = Json::nullValue;
  }
  run_record_["artifact_state"] = "complete";
  write_run_record(run_record_);
}

std::vector<Json::Value> ArtifactStore::read_complete_events(const std::filesystem::path &events_path)
{
  std::ifstream input(events_path, std::ios::binary);
  if (!input) {
    throw std::runtime_error("Unable to read event artifact: " + events_path.string());
  }
  std::vector<Json::Value> events;
  std::string line;
  while (std::getline(input, line)) {
    if (input.eof() && !input.good() && !line.empty()) {
      break;
    }
    Json::CharReaderBuilder builder;
    Json::Value value;
    std::string errors;
    std::istringstream stream(line);
    if (Json::parseFromStream(builder, stream, &value, &errors)) {
      events.push_back(std::move(value));
    }
  }
  return events;
}

void ArtifactStore::write_run_record(const Json::Value &record) const
{
  write_atomically(run_path_, render_json(record) + "\n");
}

}  // namespace navigation_evaluation