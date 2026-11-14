#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int status1, status2, exitCode;

  if (fork()) {
    wait(&status1);
    if (fork()) {
      wait(&status2);
      printf("Exit status: %d\n", status2);
    } else {
      if (execvp(argv[2], &argv[2]) == -1) {
        printf("Error executing %s", argv[2]);
        exit(1);
      }
    }
  } else {
    if (execlp("grep", "grep", "int", argv[1], NULL) == -1) {
      printf("Error executing grep");
      exit(1);
    }
  }

  return 0;
}
