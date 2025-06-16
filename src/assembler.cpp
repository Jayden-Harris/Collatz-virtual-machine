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

void file_assembler_test(const std::string& file_name, VMstate& vmstate) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file: " << file_name << std::endl;
  }
  std::string line;
  while (std::getline(file, line)) {
    std::cout << line << std::endl;
  }
}

std::vector<int> assemble_file_to_instructions(const std::string& file_name, VMstate& vmstate) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file: " << file_name << std::endl;
    return {};
  }

  std::unordered_map<std::string, int> label_table;
  std::vector<std::string> lines;
  int current_address = 0;

  while (true) {
    std::string line;
    if (!std::getline(file, line)) break;
    lines.push_back(line);

    std::vector<std::string> tokens = split_line(line);
    if (tokens.empty()) continue;

    if (tokens[0].back() == ':') {
      std::string label = tokens[0].substr(0, tokens[0].size() - 1);
      if (label_table.count(label) > 0) {
        std::cerr << "Error: Duplicate label: " << label << std::endl;
        return {};
      }
      label_table[label] = current_address;
      tokens.erase(tokens.begin());
    }

    for (const auto& token : tokens) {
      current_address++;
    }
  }

  for (const auto& [label, addr] : label_table) {
    std::cout << "Label '" << label << "' at instruction " << addr << std::endl;
  }

  std::vector<int> instructions;
  for (const auto& line : lines) {
    std::vector<std::string> tokens = split_line(line);
    if (tokens.empty()) continue;

    if (tokens[0].back() == ':') {
      tokens.erase(tokens.begin());
      if (tokens.empty()) continue;
    }

    if (!tokens.empty() && tokens[0] == "MOV" && tokens.size() == 3) {
      if (is_register(tokens[1]) && is_register(tokens[2])) {
        instructions.push_back(static_cast<int>(OPCODES::MOV_REG));
        instructions.push_back(get_register_from_string(tokens[1]));
        instructions.push_back(get_register_from_string(tokens[2]));
        continue;
      }
    }

    for (const std::string& token : tokens) {
      if (is_opcode(token)) {
        instructions.push_back(get_opcode_from_string(token));
      } else if (is_register(token)) {
        instructions.push_back(get_register_from_string(token));
      } else if (is_quoted_string(token)) {
        std::string inner = token.substr(1, token.size() - 2);
        vmstate.string_memory.push_back(inner);
        instructions.push_back(static_cast<int>(vmstate.string_memory.size()) + 2);
      } else if (label_table.count(token) > 0) {
        instructions.push_back(label_table[token]);
      } else {
        try {
          instructions.push_back(std::stoi(token));
        } catch (...) {
          std::cerr << "Error: Invalid token '" << token << "' in line: " << line << std::endl;
          return {};
        }
      }
    }
  }

  return instructions;
}
