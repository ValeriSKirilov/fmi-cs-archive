//
// Created by Lenovo on 4.11.2026 г..
//

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
  const char msg[] = "You must specify a command to execute\n";
  if (argc < 2) {
    write(1, msg, sizeof(msg) - 1);
    return 1;
  }

  const int pid = fork();

  const char error[] = "Error executing fork()\n";
  if (pid == -1) {
    write(1, error, sizeof(error) - 1);
  }

  if (pid == 0) {
    execvp(argv[1], &argv[1]);

    const char exec_error[] = "Error executing command\n";
    write(1, exec_error, sizeof(exec_error) - 1);
    return 1;
  }

  if (pid > 0) {
    int status;
    wait(&status);

    const int exit_code = (status >> 8) & 0xFF;
    printf("%d\n", exit_code);
  }

  return 0;
}
