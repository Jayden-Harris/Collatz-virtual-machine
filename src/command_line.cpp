#include "command_line.hpp"
#include <string>
#include <filesystem>
#include <vector>
#include "vmstate.hpp"
#include <unordered_map>
#include <functional>

void execute_command(std::string& command) {
  if (command == "ldir") {
    std::vector<std::filesystem::directory_entry> directories = list_directories("");
    for (const auto& entry : directories) {
      std::cout << entry.path().filename().string() << std::endl;
    }
  }
}

std::vector<std::filesystem::directory_entry> list_directories(std::string dir) {
    if (dir.empty()) {
        dir = "vm_filesystem";
    }

    std::vector<std::filesystem::directory_entry> directories;

    if (!std::filesystem::exists(dir)) {
        std::cerr << "Directory does not exist: " << dir << std::endl;
        return {};
    }

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        directories.push_back(entry);   
    }

    return directories;
}