//
// Created by Lenovo on 4.11.2026 г..
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(const int argc, char *argv[]) {
    const char msg[] = "You must specify 3 commands to execute\n";
    if (argc < 4) {
        write(1, msg, sizeof(msg) - 1);
        return 1;
    }

    const char fork_error[] = "Error executing fork()\n";
    const char exec_error[] = "Error executing command\n";

    for (int i = 1; i <= 3; i++) {
        const int pid = fork();

        if (pid == -1) {
            write(1, fork_error, sizeof(fork_error) - 1);
            return 1;
        }

        if (pid == 0) {
            execlp(argv[i], argv[i], NULL);

            write(1, exec_error, sizeof(exec_error) - 1);
            return 1;
        }

        if (pid > 0) {
            int status;
            const int child_pid = wait(&status);
            const int exit_code = (status >> 8) & 0xFF;

            printf("%d %d\n", child_pid, exit_code);
        }
    }

    return 0;
}
