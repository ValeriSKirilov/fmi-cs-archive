#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
  const char no_file_error[] =
      "No filename provided\n";  // filename -> filenames
  if (argc != 2) {               // if (argc != 3) {
    write(1, no_file_error, sizeof(no_file_error) - 1);
    return 1;
  }

  const int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  const char open_file_error[] = "Error opening file\n";
  if (fd == -1) {
    write(1, open_file_error, sizeof(open_file_error) - 1);
    exit(-1);
  }

  // ZAD 8
  const int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd2 == -1) {
    write(1, open_file_error, sizeof(open_file_error) - 1);
    exit(-1);
  }

  const int pid1 = fork();
  if (pid1 == -1) {
    exit(-1);
  }

  // 1)
  if (pid1 == 0) {
    write(fd, "Hello\n", 6);
    // ZAD 8
    if (dup2(fd2, 1) == -1) {
      exit(-1);
    }
    //
    return 0;
  } else {
    write(fd, "Hello\n", 6);
    wait(NULL);
  }

  // 2)
  if (pid1 == 0) {
    write(fd, "Hello\n", 6);
    // ZAD 8
    if (dup2(fd2, 1) == -1) {
      exit(-1);
    }
    //
    return 0;
  } else {
    const int cp_fd = open(argv[1], O_WRONLY | O_APPEND, 0644);
    if (cp_fd == -1) {
      write(1, open_file_error, sizeof(open_file_error) - 1);
      exit(-1);
    }
    write(cp_fd, "Hello\n", 6);
    wait(NULL);
  }

  // 3)
  if (pid1 == 0) {
    write(fd, "Hello\n", 6);
    // ZAD 8
    if (dup2(fd2, 1) == -1) {
      exit(-1);
    }
    //
    return 0;
  } else {
    int cp_fd = dup(fd);
    write(cp_fd, "Hello\n", 6);
    wait(NULL);
  }

  return 0;
}
