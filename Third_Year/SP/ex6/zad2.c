#include <stdio.h>
#include <unistd.h>

int main() {
  const int pid = fork();
  if (pid < 0) {
    return 1;
  }

  if (pid == 0) {
    int pidCh = fork();
  }

  printf("%d\n", getpid());
  printf("%d\n", getppid());

  return 0;
}