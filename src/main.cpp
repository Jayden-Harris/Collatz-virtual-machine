#include <stdio.h>
#include <string>
#include "vmstate.hpp"
#include "instructions.hpp"
#include <iostream>
#include "assembler.hpp"

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename.cal>" << std::endl;
    return 1;
  }

  size_t memory_size = 5000; 
  size_t number_of_registers = 19; 
  VMstate vmstate(memory_size, number_of_registers);

  std::string file_name = argv[1];

  std::cout << "Assembling file: " << file_name << std::endl;

  bool is_rewrite_enabled = true;

  if (is_rewrite_enabled) {

  } else {
      assemble_cal_file(file_name, vmstate);
  }

  run_vm(vmstate);

  return 0;
}
