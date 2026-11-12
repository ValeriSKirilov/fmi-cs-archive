//
// Created by Lenovo on 28.10.2026 г..
//

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    const int fd1 = open("ex4/zad1_helper.txt", O_RDONLY);
    const char msg[] = "Error opening file";
    if (fd1 == -1) {
        write(1, msg, sizeof(msg) - 1);
        return 1;
    }

    const int fd2 = dup(fd1);

    char buffer[1024];
    ssize_t n;

    n = read(fd1, buffer, 8);
    write(1, buffer, n);
    write(1, "\n", 1);

    n = read(fd2, buffer, 8);
    write(1, buffer, n);
    write(1, "\n", 1);

    close(fd1);

    while ((n = read(fd2, buffer, sizeof(buffer))) > 0) {
        write(1, buffer, n);
    }

    close(fd2);

    return 0;
}
