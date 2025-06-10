#include "vmstate.hpp"
#include "instructions.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "utils.hpp"
#include <vector>
#include <unordered_map>
#include <collatz.hpp>

std::vector<int> parse_file_to_integers(const std::string& file_name) {
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
  std::vector<int> integers = parse_file_to_integers(file_name);
  std::vector<std::string> parities = generate_collatz_binary_code(integers);

  std::cout << "Loading binary into memory..." << std::endl;
  vmstate.memory = parities;
}

