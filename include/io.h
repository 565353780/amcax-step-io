#pragma once

#include <string>
#include <vector>

const bool toUniqueKnotsAndMults(const std::vector<float> &knotvector,
                                 std::vector<double> &Knots,
                                 std::vector<int> &Mults);

const bool toStepFile(const std::string &json_file_path,
                      const std::string &save_step_file_path,
                      const bool &overwrite = false);
