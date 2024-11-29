#include "io.h"
#include <common/Array2.hpp>
#include <common/PointT.hpp>
#include <filesystem>
#include <geometry/Geom3BSplineSurface.hpp>
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include <step/StepWriter.hpp>
#include <topology/TopoShape.hpp>

const bool toUniqueKnotsAndMults(const std::vector<float> &knotvector,
                                 std::vector<double> &Knots,
                                 std::vector<int> &Mults) {
  if (knotvector.empty()) {
    std::cerr << "[ERROR][io::toUniqueKnotsAndMults]" << std::endl;
    std::cerr << "\t knotvector is empty!" << std::endl;

    return false;
  }

  Knots.clear();
  Mults.clear();

  int current_knot_repeat_num = 1;
  float last_knot_value = std::numeric_limits<float>().min();
  for (int i = 0; i < knotvector.size(); ++i) {
    const float current_knot_value = knotvector[i];

    if (current_knot_value == last_knot_value) {
      ++current_knot_repeat_num;

      continue;
    }

    if (last_knot_value != std::numeric_limits<float>().min()) {
      Knots.emplace_back(double(last_knot_value));
      Mults.push_back(current_knot_repeat_num);
    }

    last_knot_value = current_knot_value;
    current_knot_repeat_num = 1;
  }

  Knots.emplace_back(double(last_knot_value));
  Mults.push_back(current_knot_repeat_num);

  return true;
}

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

  std::vector<double> uKnots, vKnots;
  std::vector<int> uMults, vMults;
  toUniqueKnotsAndMults(sigmoid_knotvector_u, uKnots, uMults);
  toUniqueKnotsAndMults(sigmoid_knotvector_v, vKnots, vMults);

  AMCAX::Array2<AMCAX::Point3> poles(size_u - 1, size_v - 1);
  for (int i = 0; i < size_u - 1; ++i) {
    const int j_start_idx = i * (size_v - 1);
    for (int j = 0; j < size_v - 1; ++j) {
      const int start_idx = j_start_idx + j;

      const float x = ctrlpts[3 * start_idx];
      const float y = ctrlpts[3 * start_idx + 1];
      const float z = ctrlpts[3 * start_idx + 2];

      poles(i, j) = AMCAX::Point3(x, y, z);
    }
  }

  auto bspSrf = std::make_shared<AMCAX::Geom3BSplineSurface>(
      poles, uKnots, vKnots, uMults, vMults, degree_u, degree_v);
  std::cout << *bspSrf << std::endl;

  /*
  AMCAX::TopoShape topo_shape(bspSrf);

  const std::string save_step_folder_path =
      std::filesystem::path(save_step_file_path).parent_path();
  if (!std::filesystem::exists(save_step_folder_path)) {
    std::filesystem::create_directories(save_step_folder_path);
  }

  AMCAX::STEP::StepWriter step_writer(save_step_file_path);
  step_writer.Init();
  step_writer.WriteShape(topo_shape);
  step_writer.Done();
  */

  return true;
}
