#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// USELESS АМА ГОТИНИ
const size_t num_len(int _int) {
  int result = 1;
  while (_int / 10 != 0) {
    result++;
    _int /= 10;
  }

  return result;
}

char* itos(int _int) {
  const size_t _int_len = num_len(_int);
  char* result = malloc(_int_len + 1);
  if (result == NULL) {
    return NULL;
  }

  result[_int_len] = '\n';

  int idx = _int_len - 1;
  do {
    result[idx--] = (_int % 10) + '0';
    _int /= 10;
  } while (_int != 0);

  return result;
}
//

int main(const int argc, char* argv[]) {
  const char msg[] = "Invalid number of arguments\n";
  if (argc != 3) {
    write(1, msg, sizeof(msg) - 1);
    return 1;
  }

  const int fd_read = open(argv[1], O_RDONLY);
  if (fd_read == -1) {
    exit(-1);
  }

  const int fd_write = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd_write == -1) {
    exit(-1);
  }

  char buffer[4096];
  ssize_t n;
  size_t curr_line = 1, line_pos = 0, lines_wrote = 0;

  char line[64];
  while ((n = read(fd_read, buffer, 1024)) > 0) {
    for (ssize_t i = 0; i < n; i++) {
      line[line_pos++] = buffer[i];
      if (buffer[i] == '\n') {
        if (line_pos >= 2 && line[line_pos - 2] == '$') {
          write(1, line, line_pos);
        } else {
          for (size_t i = 0; i < line_pos; i++) {
            if (line[i] >= '0' && line[i] <= '9') {
              write(fd_write, &line[i], 1);
            }
          }
          lines_wrote++;
          write(fd_write, "\n", 1);
        }
        line_pos = 0;
        curr_line++;
      }
    }
  }

  printf("%zu\n", lines_wrote);

  return 0;
}