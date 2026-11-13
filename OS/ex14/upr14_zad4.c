#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int f = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644), status;

  if (fork()) {
    wait(&status);
    if (status == 0) {
      if (execlp("") == -1) {
        printf("Couldn't append command output");
        exit(1);
      }
    }
    if (execlp(argv[2], argv[2], NULL) == -1) {
      printf("Error executing %s", argv[2]);
      exit(1);
    }
  } else {
    if (execlp(argv[1], argv[1], NULL) == -1) {
      printf("Error executing %s", argv[1]);
      exit(1);
    }
  }

  return 0;
}
