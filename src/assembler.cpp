#include "vmstate.hpp"
#include "instructions.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "utils.hpp"
#include <vector>
#include <unordered_map>
#include <collatz.hpp>
#include "assembler.hpp"

bool is_quoted_string(const std::string& str) {
  return str.size() >= 2 && str.front() == '"' && str.back() == '"';
}

std::vector<std::string> generate_collatz_binary_code(const std::vector<int>& integers) {
  std::vector<std::string> parities;
  for (const int i : integers) {
    std::string parity = generate_collatz_parity(i);
    parities.push_back(parity + "11");
  }
  return parities;
}

void assemble_cal_file(const std::string& file_name, VMstate& vmstate) {
  std::vector<int> integers = assemble_file_to_instructions(file_name, vmstate);
  std::vector<std::string> parities = generate_collatz_binary_code(integers);
  std::cout << "Loading binary into memory..." << std::endl;
  vmstate.memory = parities;
}

std::vector<int> assemble_file_to_instructions(const std::string& file_name, VMstate& vmstate) {
  std::vector<std::string> lines;
  int total_instruction_count = 0;
  auto label_table = collect_labels_and_lines(file_name, lines, total_instruction_count);

  if (label_table.empty() && !lines.empty()) {
      // An error occurred during label collection (e.g. duplicate label)
      return {};
  }

  for (const auto& [label, addr] : label_table) {
      std::cout << "Label '" << label << "' at instruction " << addr << std::endl;
  }

  std::vector<int> instructions;
  for (const auto& line : lines) {
      auto tokens = split_line(line);
      if (tokens.empty()) continue;

      if (tokens[0].back() == ':') {
          tokens.erase(tokens.begin());
          if (tokens.empty()) continue;
      }

      if (!process_tokens_to_instructions(tokens, label_table, vmstate, instructions, line)) {
          return {};
      }
  }

  return instructions;
}

std::unordered_map<std::string, int> collect_labels_and_lines(
  const std::string& file_name,
  std::vector<std::string>& lines,
  int& total_instruction_count
) {
  std::ifstream file(file_name);
  std::unordered_map<std::string, int> label_table;
  total_instruction_count = 0;

  if (!file.is_open()) {
      std::cerr << "Error: Could not open file: " << file_name << std::endl;
      return {};
  }

  std::string line;
  while (std::getline(file, line)) {
      lines.push_back(line);

      auto tokens = split_line(line);
      if (tokens.empty()) continue;

      if (tokens[0].back() == ':') {
          std::string label = tokens[0].substr(0, tokens[0].size() - 1);
          if (label_table.count(label)) {
              std::cerr << "Error: Duplicate label: " << label << std::endl;
              return {};
          }
          label_table[label] = total_instruction_count;
          tokens.erase(tokens.begin());
      }

      total_instruction_count += static_cast<int>(tokens.size());
  }

  return label_table;
}

bool process_tokens_to_instructions(
  const std::vector<std::string>& tokens,
  const std::unordered_map<std::string, int>& label_table,
  VMstate& vmstate,
  std::vector<int>& instructions,
  const std::string& original_line
) {
  if (tokens.empty()) return true;

  if (tokens[0] == "MOV" && tokens.size() == 3 && is_register(tokens[1]) && is_register(tokens[2])) {
      instructions.push_back(static_cast<int>(OPCODES::MOV_REG));
      instructions.push_back(get_register_from_string(tokens[1]));
      instructions.push_back(get_register_from_string(tokens[2]));
      return true;
  }

  for (const auto& token : tokens) {
      if (is_opcode(token)) {
          instructions.push_back(get_opcode_from_string(token));
      } else if (is_register(token)) {
          instructions.push_back(get_register_from_string(token));
      } else if (is_quoted_string(token)) {
          std::string inner = token.substr(1, token.size() - 2);
          vmstate.string_memory.push_back(inner);
          instructions.push_back(static_cast<int>(vmstate.string_memory.size()) + 2);
      } else if (label_table.count(token)) {
          instructions.push_back(label_table.at(token));
      } else {
          try {
              instructions.push_back(std::stoi(token));
          } catch (...) {
              std::cerr << "Error: Invalid token '" << token << "' in line: " << original_line << std::endl;
              return false;
          }
      }
  }
  return true;
}
