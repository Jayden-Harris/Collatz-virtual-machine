#include "file_reader.hpp"

std::vector<std::string> read_file_lines(std::string& file_name) {
  std::vector<std::string> file_lines;
  std::ifstream file(file_name);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file: " << file_name << std::endl;
  }

  std::string line;
  while (std::getline(file, line)) {
    file_lines.push_back(line);
  }

  return file_lines;
}