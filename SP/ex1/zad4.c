//
// Created by Lenovo on 20.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
    int fds[argc];
    int totalAdded = 0;

    const char msg[] = "Error opening file\n";
    for (int i = 1; i < argc; i++) {
        const int fd = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            write(2, msg, sizeof(msg) - 1);
        } else {
            fds[totalAdded++] = fd;
        }
    }

    while (1) {
        char buffer[1024];
        const ssize_t n = read(0, buffer, sizeof(buffer));
        if (n <= 0) {
            break;
        }
        write(1, buffer, n);
        for (int i = 0; i < totalAdded; i++) {
            write(fds[i], buffer, n);
        }
    }

    for (int i = 0; i < totalAdded; i++) {
        close(fds[i]);
    }

    return 0;
}
