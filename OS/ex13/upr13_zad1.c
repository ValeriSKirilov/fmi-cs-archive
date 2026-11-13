#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int pid;

  pid = fork();
  if (pid == 0) {
    printf("par: %d\n", getpid());
  } else {
    sleep(5);
    printf("child: %d\n", getppid());
  }

  return 0;
}
