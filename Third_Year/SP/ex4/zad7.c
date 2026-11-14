#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(const int argc, char* argv[]) {
  const char msg[] = "Invalid number of arguments\n";
  if (argc != 3) {
    write(1, msg, sizeof(msg) - 1);
    return 1;
  }

  const int fd1 = open(argv[1], O_RDONLY);
  if (fd1 == -1) {
    exit(-1);
  }

  if (dup2(fd1, 0) == -1) {
    exit(-1);
  }

  const int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd2 == -1) {
    exit(-1);
  }

  if (dup2(fd2, 1) == -1) {
    exit(-1);
  }

  char buffer[4096];
  ssize_t n;

  while ((n = read(0, buffer, 1024)) > 0) {
    write(1, buffer, n);
  }

  close(fd1);
  close(fd2);

  return 0;
}