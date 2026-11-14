#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(const int argc, char* argv[]) {
  const char msg[] = "Invalid number of arguments";
  if (argc != 2) {
    write(1, msg, sizeof(msg) - 1);
    return 1;
  }

  const int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    exit(-1);
  }

  const int pid = fork();
  if (pid == -1) {
    exit(-1);
  }

  if (pid == 0) {
    write(fd, "Hello\n", 6);
    lseek(fd, 0, SEEK_SET);
    return 0;
  } else {
    wait(NULL);
    char buffer[4096];
    const ssize_t n = read(fd, buffer, 1024);
    if (n < 0) {
      exit(-1);
    }

    int counter = 2;
    for (ssize_t i = 0; i < n; ++i) {
      write(1, &buffer[i], 1);
      if (i + 1 < n) {
        write(1, " ", 1);
      }
    }
  }

  return 0;
}