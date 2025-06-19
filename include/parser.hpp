#pragma once

#include <string>
#include <vector>

// Main API: parse raw CAL source code into cleaned instruction lines
std::vector<std::string> parse_source(const std::string& rawCode, const std::string& file_name);

// Utility: remove comments from raw code (handles // and ;)
std::string strip_comments(const std::string& code);

// Utility: normalize whitespace (tabs → spaces, collapse multiple spaces, trim lines)
std::string normalize_whitespace(const std::string& code);

// Utility: split code into lines (normalize line endings too)
std::vector<std::string> split_lines(const std::string& code);

// Utility: trim leading and trailing whitespace from a string
std::string trim_whitespace(const std::string& str);

// Utility: remove commas or other separators if you want cleaner tokens downstream
std::string remove_commas(const std::string& line);