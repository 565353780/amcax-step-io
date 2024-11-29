#include "io.h"
#include <filesystem>
#include <iostream>
#include <step/StepWriter.hpp>

const bool toStepFile(const std::string &json_file_path) {
  if (!std::filesystem::exists(json_file_path)) {
    std::cerr << "[ERROR][amcax_io::toStepFile]" << std::endl;
    std::cerr << " \t json file not exist!" << std::endl;
    std::cerr << "\t json_file_path: " << json_file_path << std::endl;

    return false;
  }

  AMCAX::STEP::StepWriter writer("./output/test.step");

  return true;
}
