#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

std::string char_vector_to_string(std::vector<char> chars);

std::string trim(const std::string& s);

std::vector<std::string> split_line(const std::string& line);