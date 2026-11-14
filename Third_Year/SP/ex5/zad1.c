//
// Created by Lenovo on 4.11.2026 г..
//

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
  const char msg[] = "No file name given\n";
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
    execlp("grep", "grep", "int", argv[1], NULL);

    const char exec_error[] = "Error executing grep\n";
    write(1, exec_error, sizeof(exec_error) - 1);
    return 1;
  }

  if (pid > 0) {
    wait(NULL);
  }

  return 0;
}
