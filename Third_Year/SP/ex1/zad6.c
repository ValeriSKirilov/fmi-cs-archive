#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(const int argc, char* argv[]) {
  int fd;
  if (argc > 1) {
    fd = open(argv[1], O_RDONLY);
  } else {
    fd = 0;
  }

  if (fd == -1) {
    exit(-1);
  }

  char buffer[4096];
  ssize_t n;
  int to_read = 10;

  while ((n = read(fd, buffer, 1024)) > 0 && to_read > 0) {
    for (ssize_t i = 0; i < n; ++i) {
      if (buffer[i] == '\n') {
        to_read--;
      }
      write(1, &buffer[i], 1);
      if (to_read == 0) {
        break;
      }
    }
  }

  return 0;
}