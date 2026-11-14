#include <stdio.h>
#include <unistd.h>

int main() {
  if (fork() < 0) {
    return 1;
  }

  if (fork() < 0) {
    return 1;
  }

  printf("%d\n", getpid());
  printf("%d\n", getppid());

  return 0;
}