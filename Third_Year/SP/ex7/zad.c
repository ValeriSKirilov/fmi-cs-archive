#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

const char prompt[] = "Didi-Tensei:$ ";

void handle_sigint(int sig) {
  write(1, "\n", 1);
  signal(SIGINT, SIG_IGN);
  signal(SIGINT, handle_sigint);
}

int main() {
  const char fork_error[] = "Error creating a child process\n";
  const char exec_error[] = "Error executing the given command\n";

  char buffer[4096], result[4096];
  ssize_t n;

  signal(SIGINT, SIG_IGN);
  signal(SIGINT, handle_sigint);
  while (1) {
    ssize_t res_pos = 0;
    write(1, prompt, sizeof(prompt) - 1);

    int newline_found = 0;
    while ((n = read(0, buffer, sizeof(buffer))) > 0) {
      for (ssize_t i = 0; i < n; ++i) {
        if (buffer[i] == ' ' || buffer[i] == '\t') continue;
        if (buffer[i] == '\n') {
          result[res_pos++] = '\0';
          newline_found = 1;
          break;
        }
        result[res_pos++] = buffer[i];
      }
      if (newline_found) break;
    }

    if (res_pos == 0 && n == 0) break;
    if (n < 0) continue;

    if (res_pos == 5 && result[0] == 'q' && result[1] == 'u' &&
        result[2] == 'i' && result[3] == 't' && result[4] == '\0') {
      return 0;
    }

    if (res_pos > 0 && result[0] == '\0') continue;

    int pid = fork();
    if (pid == -1) {
      write(1, fork_error, sizeof(fork_error) - 1);
      exit(-1);
    }
    if (pid == 0) {
      signal(SIGINT, SIG_DFL);

      if (execlp(result, result, NULL) == -1) {
        write(1, exec_error, sizeof(exec_error) - 1);
        exit(-1);
      }

      return 1;
    } else {
      int status;
      wait(&status);

      const int exit_signal = status & 0x7F;
      if (exit_signal == 2) {
        write(1, "\n", 1);
      }
      const int exit_code = (status >> 8) & 0xFF;
      if (exit_code != 0) {
        continue;
      }
    }
  }

  return 0;
}