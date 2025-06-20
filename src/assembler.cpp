#include "vmstate.hpp"
#include "instructions.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "utils.hpp"
#include <vector>
#include <unordered_map>
#include <collatz.hpp>
#include "assembler.hpp"

// Checks if a string is enclosed in double quotes, like: "Hello"
bool is_quoted_string(const std::string& str) {
  return str.size() >= 2 && str.front() == '"' && str.back() == '"';
}

// Converts a list of integers into Collatz parity binary strings, appending a "11" delimiter
std::vector<std::string> generate_collatz_binary_code(const std::vector<int>& integers) {
  std::vector<std::string> parities;
  for (const int i : integers) {
    std::string parity = generate_collatz_parity(i);
    parities.push_back(parity + "11");
  }

  return parities;
}

// Main function to assemble a CAL source file:
// 1. Assembles file to raw integer instructions
// 2. Encodes them to parity strings
// 3. Stores result in VM memory
void assemble_cal_file(const std::string& file_name, VMstate& vmstate) {
  std::vector<int> integers = assemble_file_to_instructions(file_name, vmstate);
  std::vector<std::string> parities = generate_collatz_binary_code(integers);
  std::cout << "Loading binary into memory..." << std::endl;
  vmstate.memory = parities; 
}

// Converts each line in a CAL file into integer opcodes + operands
// Uses a two-pass approach: label collection first, then instruction generation
std::vector<int> assemble_file_to_instructions(const std::string& file_name, VMstate& vmstate) {
  std::vector<std::string> lines;
  int total_instruction_count = 0;

  // First pass: read file, collect lines and label positions
  auto label_table = collect_labels_and_lines(file_name, lines, total_instruction_count);

    // If file is empty or label collection failed
  if (label_table.empty() && lines.empty()) {
      return {};
  }

  std::vector<int> instructions;
  // Second pass: process each line into actual instructions
  for (const auto& line : lines) {
      auto tokens = split_line(line);              // Tokenize current line
      if (tokens.empty()) continue;                // Skip blank lines

      if (tokens[0].back() == ':') {               // If first token is a label (e.g. LOOP:)
          tokens.erase(tokens.begin());            // Remove it
          if (tokens.empty()) continue;            // Continue if nothing else on the line
      }

        // Convert tokens into instruction integers
      if (!process_tokens_to_instructions(tokens, label_table, vmstate, instructions, line)) {
          return {};
      }
  }

  return instructions;
}

// First-pass processor:
// Reads all lines, tracks total instruction count, and creates a map of labels → instruction positions
std::unordered_map<std::string, int> collect_labels_and_lines(
  const std::string& file_name,
  std::vector<std::string>& lines,
  int& total_instruction_count
) {
  std::ifstream file(file_name);
  std::unordered_map<std::string, int> label_table;
  total_instruction_count = 0;

  if (!file.is_open()) {
      std::cerr << "Error: Could not open file: " << file_name << std::endl;
      return {};
  }

  std::string line;
  while (std::getline(file, line)) {
      lines.push_back(line);                    // Store each line for second pass

      auto tokens = split_line(line);           // Tokenize
      if (tokens.empty()) continue;

      if (tokens[0].back() == ':') {            // If line starts with a label (e.g. START:)
          std::string label = tokens[0].substr(0, tokens[0].size() - 1); // Strip colon
          if (label_table.count(label)) {
              std::cerr << "Error: Duplicate label: " << label << std::endl;
              return {};
          }
          label_table[label] = total_instruction_count; // Store label's position
          tokens.erase(tokens.begin());                 // Remove label from tokens
      }

      total_instruction_count += static_cast<int>(tokens.size()); // Estimate instruction size
  }

  return label_table;
}


// Converts a list of tokens into actual instruction integers
// Handles opcodes, registers, numbers, labels, and strings
bool process_tokens_to_instructions(
  const std::vector<std::string>& tokens,
  const std::unordered_map<std::string, int>& label_table,
  VMstate& vmstate,
  std::vector<int>& instructions,
  const std::string& original_line
) {
  if (tokens.empty()) return true;

  // Remove trailing commas from tokens (e.g. "AX," → "AX")
  std::vector<std::string> cleaned_tokens;
  for (auto token : tokens) {
    if (!token.empty() && token.back() == ',') {
      token.pop_back();
    }
    cleaned_tokens.push_back(token);
  }

  // Special handling for MOV between registers (MOV AX, BX)
  if (cleaned_tokens[0] == "MOV" && cleaned_tokens.size() == 3 && 
      is_register(cleaned_tokens[1]) && is_register(cleaned_tokens[2])) {
      instructions.push_back(static_cast<int>(OPCODES::MOV_REG)); // Custom opcode
      instructions.push_back(get_register_from_string(cleaned_tokens[1]));
      instructions.push_back(get_register_from_string(cleaned_tokens[2]));
      return true;
  }

  // General-purpose parsing of each token
  for (const auto& token : cleaned_tokens) {
      if (is_opcode(token)) {
          instructions.push_back(get_opcode_from_string(token));   // Convert to opcode int
      } else if (is_register(token)) {
          instructions.push_back(get_register_from_string(token)); // Convert to register ID
      } else if (is_quoted_string(token)) {
          // Parse and decode escaped string
          std::string raw = token.substr(1, token.size() - 2);     // Remove quotes
          std::string inner;

          for (size_t i = 0; i < raw.length(); ++i) {
              if (raw[i] == '\\' && i + 1 < raw.length()) {
                  char next = raw[i + 1];
                  switch (next) {
                      case 'n': inner += '\n'; break;
                      case 't': inner += '\t'; break;
                      case 'r': inner += '\r'; break;
                      case '\\': inner += '\\'; break;
                      case '"': inner += '"'; break;
                      default: inner += next; break;
                  }
                  i++; // Skip escaped character
              } else {
                  inner += raw[i];
              }
          }

          vmstate.string_memory.push_back(inner);                   // Save to string pool
          int str_index = static_cast<int>(vmstate.string_memory.size()) - 1;
          instructions.push_back(str_index + 3);                   // Reference with offset
      } else if (label_table.count(token)) {
          instructions.push_back(label_table.at(token));           // Replace label with address
      } else {
          // Try parsing as a literal integer
          try {
              int val = std::stoi(token);
              instructions.push_back(val);
          } catch (...) {
              std::cerr << "Error: Invalid token '" << token 
                        << "' in line: " << original_line << std::endl;
              return false;
          }
      }
  }

  return true;
}
