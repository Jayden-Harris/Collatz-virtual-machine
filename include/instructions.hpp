#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

// OPCODES enum
enum class OPCODES : uint8_t {
  LOAD = 3, 
  STORE = 4, 
  MOV = 5, 
  ADD = 6, 
  SUB = 7,
  MUL = 8, 
  DIV = 9,
  OUT = 10,
  HALT = 11,
  START = 12,
  MOV_STR = 13,
  PUTS = 14,
  UNK = 0 // default for uknown opcode 
};

// Declare the opcode map as inline to avoid multiple definition errors
inline const std::unordered_map<std::string, OPCODES> opcodeMap = {
  {"LOAD", OPCODES::LOAD},
  {"STORE", OPCODES::STORE},
  {"MOV", OPCODES::MOV},
  {"ADD", OPCODES::ADD},
  {"SUB", OPCODES::SUB},
  {"MUL", OPCODES::MUL},
  {"DIV", OPCODES::DIV},
  {"OUT", OPCODES::OUT},
  {"HALT", OPCODES::HALT},
  {"START", OPCODES::START},
  {"MOV_STR", OPCODES::MOV_STR},
  {"PUTS", OPCODES::PUTS},
  {"UNK", OPCODES::UNK},
};

void handle_LOAD();
void handle_STORE();
void handle_MOV();
void handle_ADD();
void handle_SUB();
void handle_MUL();
void handle_DIV();
void handle_OUT();
void handle_HALT();
void handle_START();

uint8_t get_opcode_from_string(const std::string& s);

bool is_opcode(const std::string& token);