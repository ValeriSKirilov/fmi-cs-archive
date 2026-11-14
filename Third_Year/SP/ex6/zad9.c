#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(const int argc, char* argv[]) {
  const char msg[] = "Invalid number of arguments\n";
  if (argc != 3) {
    write(1, msg, sizeof(msg) - 1);
    exit(-1);
  }

  const int pid1 = fork();
  if (pid1 == -1) {
    exit(-1);
  }
  int pid2 = -1;
  if (pid1 != 0) {
    pid2 = fork();
    if (pid2 == -1) {
      exit(-1);
    }
  }

  if (pid1 == 0) {
    if (execlp(argv[1], argv[1], NULL) == -1) {
      exit(-1);
    }
    return 0;
  } else if (pid2 == 0) {
    if (execlp(argv[2], argv[2], NULL) == -1) {
      exit(-1);
    }
    return 0;
  } else {
    int status;
    int completed_pid = wait(&status);
    const int exit_code = (status >> 8) & 0xFF;
    if (exit_code == 0) {
      printf("%d\n", completed_pid);
    } else {
      int completed_pid_2 = wait(&status);
      const int exit_code_2 = (status >> 8) & 0xFF;
      if (exit_code_2 == 0) {
        printf("%d\n", completed_pid_2);
      } else {
        exit(-1);
      }
    }
  }

  return 0;
}