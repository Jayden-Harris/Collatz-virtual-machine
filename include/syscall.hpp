#pragma once

enum Syscall {
  SYSCALL_NOP = 50,
  SYS_CALL_LISTDIR = 51
}

void handle_syscall(int id, VMState& state);