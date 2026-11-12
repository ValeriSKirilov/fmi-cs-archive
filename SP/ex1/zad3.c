//
// Created by Lenovo on 7.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
  if (argc < 2) {
    const char msg[] = "Invalid number of arguments. A filename is required\n";
    write(2, msg, sizeof(msg) - 1);
    return 1;
  }

  const int fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (fd == -1) {
    return 1;
  }

  while (1) {
    char buffer[1024];
    const ssize_t n = read(0, buffer, sizeof(buffer));
    if (n <= 0) {
      break;
    }
    write(fd, buffer, n);
  }

  close(fd);
  return 0;
}
