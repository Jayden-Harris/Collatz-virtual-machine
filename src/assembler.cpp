#include "vmstate.hpp"
#include "instructions.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "utils.hpp"
#include <vector>
#include <unordered_map>
#include <collatz.hpp>

bool is_quoted_string(const std::string& str) {
  return str.size() >= 2 && str.front() == '"' && str.back() == '"';
}

std::vector<int> parse_file_to_integers(const std::string& file_name, VMstate& vmstate) {
  std::ifstream file(file_name);

  if (!file.is_open()) {
    std::cerr << "Error: Could not open file: " << file_name << std::endl;
    return {};
  }

  std::string line;
  std::vector<std::string> collatz_parity_streams;
  std::vector<int> integers;

  while (std::getline(file, line)) {
    std::vector<std::string> tokens = split_line(line);

    for (const std::string& token : tokens) {
      if (is_opcode(token)) {
        int opcode = get_opcode_from_string(token);
        integers.push_back(opcode);
      } else if (is_register(token)) {
        int rgstr = get_register_from_string(token);
        integers.push_back(rgstr);
      } else if (is_quoted_string(token)) {
        std::string inner = token.substr(1, token.size() - 2); // fixed
        vmstate.string_memory.push_back(inner);
        integers.push_back(vmstate.string_memory.size() + 2); // offset by 3 total
      } else {
        integers.push_back(std::stoi(token));
      }
    }
  }

  return integers;
}

std::vector<std::string> generate_collatz_binary_code(const std::vector<int>& integers) {
  std::vector<std::string> parities;

  for (const int i : integers) {
    std::string parity = generate_collatz_parity(i);
    parities.push_back(parity + "11");
  }

  return parities;
};

void assemble_cal_file(const std::string& file_name, VMstate& vmstate) {
  std::vector<int> integers = parse_file_to_integers(file_name, vmstate);
  std::vector<std::string> parities = generate_collatz_binary_code(integers);

  std::cout << "Loading binary into memory..." << std::endl;
  vmstate.memory = parities;
}
