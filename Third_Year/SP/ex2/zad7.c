//
// Created by Lenovo on 14.10.2026 г..
//

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(const int argc, char **argv) {
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

  for (int i = 5; i >= 1; i--) {
    execlp("wc", "wc", "-c", argv[1]);
    read(fd, buffer, i);
    printf("%s\n", buffer);
  }

  close(fd);

  return 0;
}
