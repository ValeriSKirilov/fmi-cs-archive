//
// Created by Lenovo on 14.10.2026 г..
//

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int main() {
  const int fd = open("ex2/aa", O_RDONLY);
  if (fd == -1) {
    perror("Error opening file");
    return 1;
  }

  char buffer[16];

  for (int i = 5; i >= 1; i--) {
    lseek(fd, -3, SEEK_END);
    read(fd, buffer, i);
    // buffer[i] = '\0';
    printf("%s\n", buffer);
  }

  close(fd);

  return 0;
}
