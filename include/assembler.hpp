#pragma once

#include "vmstate.hpp"
#include "instructions.hpp"

bool is_quoted_string(const std::string& str);

void assemble_cal_file(const std::string& file_name, VMstate& vmstate);

std::vector<int> assemble_file_to_instructions(const std::string& file_name, VMstate& vmstate);

std::vector<std::string> generate_collatz_binary_code(const std::vector<int>& integers);

std::unordered_map<std::string, int> collect_labels_and_lines(
  const std::string& file_name,
  std::vector<std::string>& lines,
  int& total_instruction_count
);


bool process_tokens_to_instructions(
  const std::vector<std::string>& tokens,
  const std::unordered_map<std::string, int>& label_table,
  VMstate& vmstate,
  std::vector<int>& instructions,
  const std::string& original_line
);