//
// Created by Lenovo on 7.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
  if (argc != 3) {
    return 0;
  }

  const int f1 = open(argv[1], O_RDONLY);
  if (f1 == -1) {
    return 0;
  }

  const int f2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (f2 == -1) {
    return 0;
  }

  ssize_t n;
  int buff[1024];
  while ((n = read(f1, buff, 4096)) > 0) {
    write(f2, buff, n);
  }

  close(f1);
  close(f2);

  return 0;
}
