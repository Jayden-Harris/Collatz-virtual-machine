#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

std::string char_vector_to_string(std::vector<char> chars) {
  std::string s(chars.begin(), chars.end());
  return s;
}

std::string trim(const std::string& s) {
  if (s.empty()) return "";

  auto start = s.begin();
  while (start != s.end() && std::isspace(*start)) {
      start++;
  }

  auto end = s.end();
  do {
      end--;
  } while (std::distance(start, end) > 0 && std::isspace(*end));

  return std::string(start, end + 1);
}

std::vector<std::string> split_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;
    while (stream >> token) {
        // Remove trailing commas, if any
        if (!token.empty() && token.back() == ',') {
            token.pop_back();
        }
        tokens.push_back(trim(token));
    }
    return tokens;
  }


