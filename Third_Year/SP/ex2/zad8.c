//
// Created by Lenovo on 14.10.2026 г..
//

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
  if (argc < 2) {
    perror("Invalid number of arguments. A filename is required");
    return 1;
  }

  const int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("Error opening file");
    return 1;
  }

  char buffer[16];
  ssize_t n;

  while ((n = read(fd, buffer, 10)) > 0) {
    printf("%zd\n", n);
  }

  close(fd);

  return 0;
}
