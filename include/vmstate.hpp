#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <iostream>

enum class Registers : uint8_t {
  AX = 13,
  BX = 14,
  CX = 15,
  DX = 16,
  IP = 17,
  SP = 18,
};

inline const std::unordered_map<std::string, Registers> registerMap = {
  {"AX", Registers::AX},
  {"BX", Registers::BX},
  {"CX", Registers::CX},
  {"DX", Registers::DX},
  {"IP", Registers::IP},
  {"SP", Registers::SP}
};

struct VMstate {
  std::vector<std::string> memory;   
  std::vector<int> registers;
  uint64_t pc;

  VMstate(size_t memory_size, size_t number_of_registers)
    : memory(memory_size, ""), registers(number_of_registers, 0), pc(0) {}
};

void run_vm(VMstate& vmstate);

uint8_t get_register_from_string(const std::string& reg_str);



bool is_register(const std::string& token);

std::vector<int> consume_collatz_binary(const std::vector<std::string>& paraties);

std::vector<std::string> clean_collatz_binary(const std::vector<std::string>& collatz_binary);

void execute_program(VMstate& vmstate, std::vector<int> instructions);