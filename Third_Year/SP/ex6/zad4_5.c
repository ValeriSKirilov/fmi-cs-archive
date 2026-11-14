#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int main(const int argc, char* argv[]) {
  const char msg[] = "Invalid number of arguments\n";
  if (argc < 3) {
    write(1, msg, sizeof(msg) - 1);
    return 1;
  }

  const int fd1 = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd1 == -1) {
    return 1;
  }

  const int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd2 == -1) {
    return 1;
  }

  const int pid = fork();

  if (pid == 0) {
    const int saved_stdout = dup(1);
    dup2(fd2, 1);

    write(fd1, "hello2\n", 7);

    dup2(saved_stdout, 1);
    close(saved_stdout);
  } else {
    // wait(NULL);
    write(fd1, "hello1\n", 7);
  }

  write(1, "hello3\n", 7);
  write(fd2, "hello4\n", 7);

  return 0;
}