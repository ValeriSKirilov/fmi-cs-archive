#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(const int argc, char* argv[]) {
  const char msg[] = "Invalid number of arguments\n";
  if (argc != 3) {
    write(1, msg, sizeof(msg) - 1);
    return 1;
  }

  const int pid = fork();
  if (pid == -1) {
    exit(-1);
  }

  int status;
  if (pid == 0) {
    if (execlp(argv[1], argv[1], NULL) == -1) {
      exit(-1);
    }
    return 0;
  } else {
    wait(&status);
  }

  const int exit_code = (status >> 8) & 0xFF;
  if (exit_code != 0) {
    return -1;
  }

  if (execlp(argv[2], argv[2], NULL) == -1) {
    exit(-1);
  }

  return 0;
}