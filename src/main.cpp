#include "io.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <string1> <string2>" << std::endl;
    return 1;
  }

  // const std::string json_folder_path = "../bspline-fitting/output/";
  // const std::string save_step_folder_path = "../bspline-fitting/output/";
  const std::string json_file_path = argv[1];
  const std::string save_step_file_path = argv[2];
  const bool overwrite = true;

  std::cout << "start convert [" << json_file_path << "] --> ["
            << save_step_file_path << "]" << std::endl;
  if (!toStepFile(json_file_path, save_step_file_path, overwrite)) {
    std::cerr << "\t convert failed!" << std::endl;

    return -1;
  }

  std::cerr << "\t convert success!" << std::endl;

  return 1;
}
