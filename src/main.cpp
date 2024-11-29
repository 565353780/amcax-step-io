#include "io.h"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  // 检查参数数量（程序名 + 两个参数 = 3）
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <string1> <string2>" << std::endl;
    return 1;
  }

  // const std::string json_folder_path = "../bspline-fitting/output/";
  // const std::string save_step_folder_path = "../bspline-fitting/output/";
  const std::string json_folder_path = argv[1];
  const std::string save_step_folder_path = argv[2];
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
