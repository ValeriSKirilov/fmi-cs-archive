#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(const int argc, char* argv[]) {
  // ZAD 6
  const char msg[] = "Invalid number of arguments\n";
  if (argc != 2) {
    write(1, msg, sizeof(msg) - 1);
    exit(-1);
  }

  const int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    exit(-1);
  }
  //

  write(1, "EXAMPLE\n", 8);

  // ZAD 6
  const int default_output_save = dup(1);
  if (default_output_save == -1) {
    exit(-1);
  }
  dup2(fd, 1);
  //

  for (int i = 1; i <= 10; ++i) {
    if (i == 10) {
      write(1, "10\n", 3);
    } else {
      char buf[2];
      buf[0] = i + '0';
      buf[1] = '\n';
      write(1, buf, 2);
    }
  }

  // ZAD 6
  dup2(default_output_save, 1);
  //

  write(1, "HELLO\n", 6);

  return 0;
}