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
    std::string current_token;
    bool in_quotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"') {
            current_token += c;
            in_quotes = !in_quotes; 
        }
        else if (std::isspace(c) && !in_quotes) {
            if (!current_token.empty()) {
                // Remove trailing commas if any
                if (current_token.back() == ',') current_token.pop_back();
                tokens.push_back(current_token);
                current_token.clear();
            }
        }
        else {
            current_token += c;
        }
    }

    if (!current_token.empty()) {
        if (current_token.back() == ',') current_token.pop_back();
        tokens.push_back(current_token);
    }

    return tokens;
}


