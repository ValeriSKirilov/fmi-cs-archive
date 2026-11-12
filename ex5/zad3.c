//
// Created by Lenovo on 4.11.2026 г..
//

#include <unistd.h>
#include <sys/wait.h>

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
        execlp(argv[1], argv[1], NULL);

        const char exec_error[] = "Error executing command\n";
        write(1, exec_error, sizeof(exec_error) - 1);
        return 1;
    }

    if (pid > 0) {
        int status;
        wait(&status);

        const int exit_code = (status >> 8) & 0xFF;
        if (exit_code == 0) {
            write(1, argv[1], sizeof(argv[1]));
        }
    }

    return 0;
}
