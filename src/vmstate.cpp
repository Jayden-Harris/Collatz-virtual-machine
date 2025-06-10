#include <stdio.h>
#include <string>
#include <vmstate.hpp>
#include <vector>
#include "collatz.hpp"
#include <iostream>
#include <fstream>
#include "instructions.hpp"

uint8_t get_register_from_string(const std::string& reg_str) {
  auto it = registerMap.find(reg_str);
  if (it != registerMap.end()) {
    return static_cast<uint8_t>(it->second);
  }

  std::cerr << "Invalid register: " << reg_str << std::endl;
  return UINT8_MAX; 
}


bool is_register(const std::string& token) {
  return registerMap.count(token) > 0;
}

void run_vm(VMstate& vmstate) {
  std::cout << "Initializing VM..." << std::endl;
  std::vector<std::string> collatz_binary = vmstate.memory;
  std::vector<std::string> cleaned_binary = clean_collatz_binary(collatz_binary);
  std::vector<int> instructions = consume_collatz_binary(cleaned_binary);

  execute_program(vmstate, instructions);
  
}

std::vector<int> consume_collatz_binary(const std::vector<std::string>& collatz_binary) {

  std::vector<int> instructions;

  for (const std::string& parity : collatz_binary) {
    int n = reverse_collatz_parity(parity);
    if (n == -1) {
      std::cerr << "Failed to reverse parity string: " << parity << std::endl;
      continue;
    }
    instructions.push_back(static_cast<int>(n));
  }

  return instructions;
}

std::vector<std::string> clean_collatz_binary(const std::vector<std::string>& collatz_binary) {
  std::vector<std::string> cleaned;

  for (const std::string& bin : collatz_binary) {
    if (bin.size() >= 2 && bin.substr(bin.size() - 2) == "11") {
        cleaned.push_back(bin.substr(0, bin.size() - 2));
    } else {
        cleaned.push_back(bin); 
    }
  }

  return cleaned;
}


// main program loop to read instructions and then execute code 
void execute_program(VMstate& vmstate, std::vector<int> instructions) {
  size_t ip = 0;
  std::cout << "Program Output: \n" << std::endl;

  while (ip < instructions.size()) {
    int opcode = instructions[ip++];

    switch(opcode) {
      case 5: { // MOV
        int reg = instructions[ip++];
        int val = instructions[ip++];
        vmstate.registers[reg] = val;
        break;
      }

      case 6: { // ADD
        int reg1 = instructions[ip++];
        int reg2 = instructions[ip++];
        vmstate.registers[reg1] += vmstate.registers[reg2];
        break;
      }

      case 7: { // SUB
        int reg1 = instructions[ip++];
        int reg2 = instructions[ip++];
        vmstate.registers[reg1] -= vmstate.registers[reg2];
        break;
      }

      case 10: { // OUT
        int reg = instructions[ip++];  // Register index to output
        int val = vmstate.registers[reg];
        std::cout << val << std::endl;
        break;
      }

      case 11: { // HALT
        std::cout << "\nProgram finished with exit code 0" << std::endl;
        break;
      }
    }
  }
}