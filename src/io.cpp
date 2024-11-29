#include "io.h"
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <step/StepWriter.hpp>

const bool toStepFile(const std::string &json_file_path,
                      const std::string &save_step_file_path,
                      const bool &overwrite) {
  if (!std::filesystem::exists(json_file_path)) {
    std::cerr << "[ERROR][amcax_io::toStepFile]" << std::endl;
    std::cerr << " \t json file not exist!" << std::endl;
    std::cerr << "\t json_file_path: " << json_file_path << std::endl;

    return false;
  }

  if (!overwrite) {
    if (std::filesystem::exists(save_step_file_path)) {
      return true;
    }

    std::filesystem::remove(save_step_file_path);
  }

  std::ifstream inputFile(json_file_path);
  if (!inputFile.is_open()) {
    std::cerr << "[ERROR][io::toStepFile]" << std::endl;
    std::cerr << "\t failed to open json file" << std::endl;
    std::cerr << "\t json_file_path: " << json_file_path << std::endl;

    return false;
  }

  nlohmann::json jsonData;
  try {
    inputFile >> jsonData;
  } catch (const nlohmann::json::parse_error &e) {
    inputFile.close();

    std::cerr << "[ERROR][io::toStepFile]" << std::endl;
    std::cerr << "\t convert to json failed!" << std::endl;
    std::cerr << "\t JSON parse error: " << e.what() << std::endl;

    return false;
  }

  inputFile.close();

  int degree_u;
  int degree_v;
  int size_u;
  int size_v;
  int sample_num_u;
  int sample_num_v;
  int start_u;
  int start_v;
  int stop_u;
  int stop_v;
  std::string idx_dtype;
  std::string dtype;
  std::string device;
  std::vector<float> knots_u;
  std::vector<float> knots_v;
  std::vector<float> knotvector_u;
  std::vector<float> knotvector_v;
  std::vector<float> sigmoid_knotvector_u;
  std::vector<float> sigmoid_knotvector_v;
  std::vector<float> ctrlpts;

  try {
    degree_u = jsonData.at("degree_u").get<int>();
    degree_v = jsonData.at("degree_v").get<int>();
    size_u = jsonData.at("size_u").get<int>();
    size_v = jsonData.at("size_v").get<int>();
    sample_num_u = jsonData.at("sample_num_u").get<int>();
    sample_num_v = jsonData.at("sample_num_v").get<int>();
    start_u = jsonData.at("start_u").get<float>();
    start_v = jsonData.at("start_v").get<float>();
    stop_u = jsonData.at("stop_u").get<float>();
    stop_v = jsonData.at("stop_v").get<float>();
    idx_dtype = jsonData.at("idx_dtype").get<std::string>();
    dtype = jsonData.at("dtype").get<std::string>();
    device = jsonData.at("device").get<std::string>();
    knots_u = jsonData.at("knots_u").get<std::vector<float>>();
    knots_v = jsonData.at("knots_v").get<std::vector<float>>();
    knotvector_u = jsonData.at("knotvector_u").get<std::vector<float>>();
    knotvector_v = jsonData.at("knotvector_v").get<std::vector<float>>();
    sigmoid_knotvector_u =
        jsonData.at("sigmoid_knotvector_u").get<std::vector<float>>();
    sigmoid_knotvector_v =
        jsonData.at("sigmoid_knotvector_v").get<std::vector<float>>();
    ctrlpts = jsonData.at("ctrlpts").get<std::vector<float>>();

  } catch (const nlohmann::json::out_of_range &e) {
    std::cerr << "[ERROR][io::toStepFile]" << std::endl;
    std::cerr << "\t load json data failed!" << std::endl;
    std::cerr << "\t JSON key error: " << e.what() << std::endl;

    return false;
  }

  const std::string save_step_folder_path =
      std::filesystem::path(save_step_file_path).parent_path();
  if (!std::filesystem::exists(save_step_folder_path)) {
    std::filesystem::create_directories(save_step_folder_path);
  }

  AMCAX::STEP::StepWriter step_writer(save_step_file_path);
  step_writer.Init();

  return true;
}
