#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <iostream>

enum class Registers : uint8_t {
  AX = 20,
  BX = 21,
  CX = 22,
  DX = 23,
  EX = 24,
  FX = 25,
  GX = 26,
};

inline const std::unordered_map<std::string, Registers> registerMap = {
  {"AX", Registers::AX},
  {"BX", Registers::BX},
  {"CX", Registers::CX},
  {"DX", Registers::DX},
  {"EX", Registers::EX},
  {"FX", Registers::FX},
  {"GX", Registers::GX},
  {"IP", Registers::IP},
  {"SP", Registers::SP}
};

struct VMstate {
  std::vector<std::string> memory;   
  std::vector<std::string> string_memory; 
  std::vector<int> registers;
  uint64_t pc;

  VMstate(size_t memory_size, size_t number_of_registers)
    : memory(memory_size, ""), registers(number_of_registers, 0), pc(0) {}
};

void run_vm(VMstate& vmstate);

uint8_t get_register_from_string(const std::string& reg_str);

void execute_program(VMstate& vmstate, const std::vector<int> instructions);

bool is_register(const std::string& token);

std::vector<int> consume_collatz_binary(const std::vector<std::string>& collatz_binary);

std::vector<std::string> clean_collatz_binary(const std::vector<std::string>& collatz_binary);
