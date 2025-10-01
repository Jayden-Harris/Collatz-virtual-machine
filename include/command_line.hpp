#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include "vmstate.hpp"


void execute_command(const std::string& command, const std::optional<std::string>& argument = std::nullopt);

std::vector<std::filesystem::directory_entry> list_directories(std::string dir);

bool createDirectory(const std::string& path);