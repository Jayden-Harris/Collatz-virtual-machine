#pragma once


#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdio.h>

enum class Syscalls : uint8_t {
  NOP = 50,
  LDIR = 51
};

inline const std::unordered_map<std::string, Syscalls> syscallMap = {
  {"NOP", Syscalls::NOP},
  {"LDIR", Syscalls::LDIR}
};

void handle_syscall(int id);

void list_dir_handler();

bool is_syscall(const std::string& token);

uint8_t get_syscall_from_string(const std::string& token);