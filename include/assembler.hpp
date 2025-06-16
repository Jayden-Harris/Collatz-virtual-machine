#pragma once

#include "vmstate.hpp"
#include "instructions.hpp"

bool is_quoted_string(const std::string& str);

void assemble_cal_file(const std::string& file_name, VMstate& vmstate);

void file_assembler_test(const std::string& file_name, VMstate& vmstate);

std::vector<int> assemble_file_to_instructions(const std::string& file_name, VMstate& vmstate);

std::vector<std::string> generate_collatz_binary_code(const std::vector<int>& integers);


