#include <stdio.h>
#include "instructions.hpp"
#include "vmstate.hpp"

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

uint8_t get_opcode_from_string(const std::string& opcode_string) {
  auto it = opcodeMap.find(opcode_string);
    if (it != opcodeMap.end()) {
        return static_cast<int>(it->second);
    } 

    return 0;
}

bool is_opcode(const std::string& token) {
  return opcodeMap.count(token) > 0;
}

