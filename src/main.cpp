#include "io.h"
#include <filesystem>
#include <iostream>
#include <string>

int main() {
  const std::string json_folder_path = "../bspline-fitting/output/";
  const std::string save_step_folder_path = "../bspline-fitting/output/";
  const bool overwrite = true;

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(json_folder_path)) {
    const std::filesystem::path file_path = entry.path();

    if (entry.is_regular_file() && file_path.extension() == ".json") {
      const std::string save_step_file_path =
          save_step_folder_path + file_path.stem().string() + ".step";

      std::cout << "start convert [" << file_path.string() << "] --> ["
                << save_step_file_path << "]" << std::endl;
      toStepFile(file_path, save_step_file_path, overwrite);
    }
  }

  return 1;
}
