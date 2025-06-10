# CollatzVM

A Collatz-Based Virtual Machine & Encoder Framework implementing a unique assembly language and binary format based on Collatz parity encoding (CPE).  
This project explores novel encoding techniques inspired by the Collatz conjecture and builds a minimalistic VM that runs Collatz binary programs.

---

## Features

- **Custom Instruction Set**: Design and run programs with a simple assembly-like language (`.cal` files).  
- **Collatz Parity Encoding (CPE)**: Encode and decode binary instructions using Collatz parity sequences.  
- **Assembler & Runtime**: Compile `.cal` source files into a collatz binary parity string which is stored in memory and then execute by the VM
- **Modular Codebase**: Organized into separate modules for VM, assembler, Collatz encoding, I/O, and utilities.  
- **Extensible Architecture**: Easy to add new instructions, I/O devices, and debugging tools.

---

## Example .cal file

```bash
START

MOV AX, 20
MOV BX, 10
SUB AX, BX
OUT AX

HALT

```

```bash
Output: 10
```

## Getting Started

### Prerequisites

- C++17 compatible compiler (e.g., g++, clang++)  
- [Boost Multiprecision Library](https://www.boost.org/doc/libs/release/libs/multiprecision/)  
- CMake (optional, recommended for building)

### Building

```bash
git clone https://github.com/Jayden-Harris/CollatzVM.git
cd CollatzVM
mkdir build && cd build
cmake ..
make
