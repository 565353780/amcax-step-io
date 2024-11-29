#pragma once

#include <string>

const bool toStepFile(const std::string &json_file_path,
                      const std::string &save_step_file_path,
                      const bool &overwrite = false);
