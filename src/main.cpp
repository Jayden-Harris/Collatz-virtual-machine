#include <stdio.h>
#include <string>
#include "vmstate.hpp"
#include "instructions.hpp"
#include <iostream>
#include "assembler.hpp"
#include <cmath>
#include <sstream>
#include <thread>
#include <chrono>
#include <cstdlib>

void print_intro_screen() {
  system("clear");
  std::cout << "\033[32m" << R"(


--------------------------------------------------------------
║   ☢️  СИСТЕМА РЕАКТОРА КОЛЛАТЦ - COLLatz REACTOR SYSTEM  ☢️  ║
--------------------------------------------------------------

[СТАТУС]: Реактор: ОТКЛЮЧЕН (REACTOR OFFLINE)
[СТАТУС]: ВМ-система: НЕ ЗАПУЩЕНА (VM SYSTEM INACTIVE)
[ИНФО]: Введите команду для запуска системы...
[INFO]: Type 'load <filename>' to execute reactor instructions.

------------------------------------------------------------

)" << std::endl;
}

int main(int argc, char* argv[]) {

  print_intro_screen();

  size_t memory_size = 5000;
  size_t number_of_registers = 25;

  while (true) {
      std::cout << "команда> ";
      std::string input;
      std::getline(std::cin, input);

      // Trim whitespace
      if (input.empty()) continue;

      std::istringstream iss(input);
      std::string command;
      iss >> command;

      if (command == "exit" || command == "quit") {
          std::cout << "[Выход]: Shutting down reactor CLI...\n";
          break;
      }

      if (command == "load") {
        std::string filename;
        iss >> filename;
        if (filename.empty()) {
            std::cout << "[Ошибка]: No filename provided.\n";
            continue;
        }

        std::cout << "[ЗАГРУЗКА]: Assembling file: " << filename << std::endl;

        VMstate vmstate(memory_size, number_of_registers);
        assemble_cal_file(filename, vmstate);

        std::cout << "[ГОТОВО]: Assembly complete. Executing reactor code...\n";
        run_vm(vmstate);

        std::cout << "[ЗАВЕРШЕНО]: Execution complete. Reactor awaits new instructions.\n";
        continue;
      } else if (command == "status") {
        std::cout << "\033[1;31m"
        << "СТАТУС СИСТЕМЫ: НЕИЗВЕСТЕН\n"
        << "РЕАКТОР: ВЫКЛЮЧЕН — REACTOR STATUS: OFFLINE\n"
        << "МОЩНОСТЬ: 0 МВт — POWER OUTPUT: 0 MW"
        << "\033[0m" << std::endl;
    } else {
      std::cout << "[Неизвестная команда]: Unknown command: " << command << std::endl;
      std::cout << "[INFO]: Available commands: load <file>, exit\n";
    }
  }

  return 0;
}