#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include "vmstate.hpp"


void execute_command(std::string& command);

std::vector<std::filesystem::directory_entry> list_directories(std::string dir);