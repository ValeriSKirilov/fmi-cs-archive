//
// Created by Lenovo on 21.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>

int main() {
  const char opening_file_error[] = "Error opening file.\n";

  const int fd = open("ex3/fff", O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    write(1, opening_file_error, sizeof(opening_file_error) - 1);
    return 1;
  }
  close(fd);

  const int fd1 = open("ex3/fff", O_RDONLY);
  if (fd1 == -1) {
    write(1, opening_file_error, sizeof(opening_file_error) - 1);
    return 1;
  }

  const int fd2 = open("ex3/fff", O_WRONLY);
  if (fd2 == -1) {
    write(1, opening_file_error, sizeof(opening_file_error) - 1);
    return 1;
  }

  const int fd3 = open("ex3/fff", O_RDWR);
  if (fd3 == -1) {
    write(1, opening_file_error, sizeof(opening_file_error) - 1);
    return 1;
  }

  // A)
  // const char hello_word[] = "Hello world";
  // write(fd2, hello_word, sizeof(hello_word) - 1);
  //
  // char bufferFd3_6b[6];
  // const ssize_t nFd3_6b = read(fd3, bufferFd3_6b, sizeof(bufferFd3_6b));
  // if (nFd3_6b > 0)
  //     write(1, bufferFd3_6b, nFd3_6b);
  //
  // char bufferFd1_5b[5];
  // const ssize_t nFd1_5b = read(fd1, bufferFd1_5b, sizeof(bufferFd1_5b));
  // if (nFd1_5b > 0)
  //     write(1, bufferFd1_5b, nFd1_5b);
  //
  // const char chil[] = "chil";
  // write(fd3, chil, sizeof(chil) - 1);
  //
  // const char exclam[] = "!!!";
  // write(fd2, exclam, sizeof(exclam) - 1);
  //
  // char bufferFd1_9b[9];
  // const ssize_t nFd1_9b = read(fd1, bufferFd1_9b, sizeof(bufferFd1_9b));
  // if (nFd1_9b)
  //     write(1, bufferFd1_9b, nFd1_9b);

  // B)
  const char hello[] = "Hello";
  write(fd3, hello, sizeof(hello) - 1);

  const char worlds[] = "worlds";
  write(fd2, worlds, sizeof(worlds) - 1);

  char bufferFd1_6b[6];
  const ssize_t nFd1_6b = read(fd1, bufferFd1_6b, sizeof(bufferFd1_6b));
  if (nFd1_6b > 0) write(1, bufferFd1_6b, nFd1_6b);

  close(fd2);

  const char oops[] = "oops";
  write(fd3, oops, sizeof(oops) - 1);

  char bufferFd1_6b_v2[6];
  const ssize_t nFd1_6b_v2 =
      read(fd1, bufferFd1_6b_v2, sizeof(bufferFd1_6b_v2));
  if (nFd1_6b_v2 > 0) write(1, bufferFd1_6b_v2, nFd1_6b_v2);

  close(fd1);
  close(fd2);
  close(fd3);

  return 0;
}
