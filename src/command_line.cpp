#include "command_line.hpp"
#include <string>
#include <filesystem>
#include <vector>
#include "vmstate.hpp"
#include <unordered_map>
#include <functional>
#include <optional>

void execute_command(const std::string& command, const std::optional<std::string>& argument) {
  if (command == "ldir") {
    std::string dir = argument.value_or(""); // empty string if no argument provided
    std::vector<std::filesystem::directory_entry> directories = list_directories(dir);
    for (const auto& entry : directories) {
        std::cout << entry.path().filename().string() << std::endl;
    }
  } else if (command == "mkdir") {
    if (!argument.has_value() || argument->empty()) {
        std::cerr << "Error: mkdir requires a directory name!" << std::endl;
    } else {
        createDirectory(*argument);
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

bool createDirectory(const std::string& path) {
  if (path.empty()) {
    std::cout << "Error: Please provide a path name to MKDIR" << std::endl;
    return false; 
  }
  try {
    // Try creating the directory
    if (std::filesystem::create_directory(path)) {
      return true;
    } else {
      std::cout << "ERROR: Directory already exists: " << path << std::endl;
      return false;
    }
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << "Error creating directory: " << e.what() << std::endl;
    return false;
  }
}