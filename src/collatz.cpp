#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <collatz.hpp>
#include "utils.hpp"

std::string generate_collatz_parity(int n) {
  std::vector<char> parity;

  while (n != 1) {
    if (n % 2 == 0) {
      parity.push_back('0');
      n = n / 2;
    } else {
      parity.push_back('1');
      n = 3 * n + 1;
    }
  }

  return char_vector_to_string(parity);
}

int reverse_collatz_parity(std::string parity) {
  int n = 1;
  int prev;
  std::reverse(parity.begin(), parity.end());

  for (char c : parity) {
    if (c == '0') {
      n = n * 2;
    } else if (c == '1') {
      if ((n-1) % 3 != 0) {
        return -1;
      }
      prev = (n-1) / 3;
      if (prev % 2 == 0) {
        return -1;
      }
      n = prev;
    } else {
      return -1;
    }
  }
  return n;
}
