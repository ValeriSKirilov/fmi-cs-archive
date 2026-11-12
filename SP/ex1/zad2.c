//
// Created by Lenovo on 7.10.2026 г..
//

#include <unistd.h>

int main() {
  while (1) {
    char buffer[1024];
    const ssize_t n = read(0, buffer, sizeof(buffer));
    if (n <= 0) {
      break;
    }
    write(1, buffer, n);
  }

  return 0;
}
