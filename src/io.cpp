#include "io.h"
#include <filesystem>
#include <iostream>
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

  const std::string save_step_folder_path =
      std::filesystem::path(save_step_file_path).parent_path();
  if (!std::filesystem::exists(save_step_folder_path)) {
    std::filesystem::create_directories(save_step_folder_path);
  }

  AMCAX::STEP::StepWriter step_writer(save_step_file_path);
  step_writer.Init();

  return true;
}
