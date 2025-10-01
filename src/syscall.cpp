#include "syscall.hpp"
#include <stdio.h>
#include <iostream>

void handle_syscall(int id) {
 Syscalls syscall = static_cast<Syscalls>(id);
 switch(syscall) {
  case Syscalls::LDIR: {
    list_dir_handler();
  }
 }
};

bool is_syscall(const std::string& token) {
  return syscallMap.count(token) > 0;
}

uint8_t get_syscall_from_string(const std::string& token) {
  auto it = syscallMap.find(token);

  if (it != syscallMap.end()) {
    return static_cast<int>(it->second);
  }

  return UINT8_MAX;
}

void list_dir_handler() {
  std::string dir = "vm_filesystem";

  std::vector<std::filesystem::directory_entry> directories;

  if (!std::filesystem::exists(dir)) {
    std::cout << "Directory does not exist: " << dir << std::endl;
  }

  for (const auto& entry : std::filesystem::directory_iterator(dir)) {
    directories.push_back(entry);   
  }

  for (const auto& entry : directories) {
    std::cout << entry.path().filename().string() << std::endl;
  }
}
