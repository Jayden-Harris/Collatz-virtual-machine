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

void execute_program(VMstate& vmstate, std::vector<int> instructions) {
  size_t ip = 0;
  std::cout << "Program Output: \n" << std::endl;

  while (ip < instructions.size()) {
    OPCODES opcode = static_cast<OPCODES>(instructions[ip++]);

    switch (opcode) {
      case OPCODES::START: {
        break; 
      }

      case OPCODES::MOV: {
        int reg = instructions[ip++];
        int val = instructions[ip++];
        vmstate.registers[reg] = val;
        break;
      }

      case OPCODES::ADD: {
        int reg1 = instructions[ip++];
        int reg2 = instructions[ip++];
        vmstate.registers[reg1] += vmstate.registers[reg2];
        break;
      }

      case OPCODES::SUB: {
        int reg1 = instructions[ip++];
        int reg2 = instructions[ip++];
        vmstate.registers[reg1] -= vmstate.registers[reg2];
        break;
      }

      case OPCODES::OUT: {
        int reg = instructions[ip++];
        int val = vmstate.registers[reg];
        std::cout << val << std::endl;
        break;
      }

      case OPCODES::PUTS: {
        int str_index = instructions[ip++] - 3;
        if (str_index >= 0 && str_index < vmstate.string_memory.size()) {
            std::cout << vmstate.string_memory[str_index] << std::endl;
        } else {
            std::cerr << "Invalid string index: " << (str_index + 3) << std::endl;
        }
        break;
      }

      case OPCODES::JMP: {
		    int addr = instructions[ip++];
		    ip = addr;
		    continue;
		  }

      case OPCODES::JNZ: {
        int reg = instructions[ip++];
        int addr = instructions[ip++];
        if (vmstate.registers[reg] != 0) {
          ip = addr;
          continue;
        }
        break;
      }

      case OPCODES::HALT: {
        std::cout << "\nProgram finished with exit code 0" << std::endl;
        return; 
      }

      case OPCODES::MOV_REG: {
        int dest = instructions[ip++];
        int src = instructions[ip++];
        vmstate.registers[dest] = vmstate.registers[src];
        break;
      }

      default: {
        std::cerr << "Unknown opcode: " << static_cast<int>(opcode) << std::endl;
        break;
      }
    }
  }
}
