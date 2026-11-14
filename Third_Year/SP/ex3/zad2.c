//
// Created by Lenovo on 21.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>

int main() {
  const char opening_file_error[] = "Error opening file\n";

  const int fd1 = open("ex3/f1", O_RDWR);
  if (fd1 == -1) {
    write(1, opening_file_error, sizeof(opening_file_error) - 1);
    return 1;
  }
  const int fd2 = open("ex3/f1", O_RDWR);
  if (fd2 == -1) {
    write(1, opening_file_error, sizeof(opening_file_error) - 1);
    return 1;
  }

  lseek(fd1, -2, SEEK_END);
  lseek(fd2, 4, 0);

  const char wxyz[] = "wxyz";
  write(fd1, wxyz, sizeof(wxyz) - 1);

  const char onetwo[] = "12";
  write(fd2, onetwo, sizeof(onetwo) - 1);

  close(fd1);
  close(fd2);

  execlp("cat", "cat", "ex3/f1", NULL);

  return 0;
}
