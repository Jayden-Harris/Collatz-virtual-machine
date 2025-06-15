#pragma once

#include "vmstate.hpp"
#include "instructions.hpp"

bool is_quoted_string(const std::string& str);

void assemble_cal_file(const std::string& file_name, VMstate& vmstate);

std::vector<int> parse_file_to_integers(const std::string& file_name);

std::vector<std::string> generate_collatz_binary_code(const std::vector<int>& integers);


