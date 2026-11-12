//
// Created by Lenovo on 28.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("ex4/f1", O_WRONLY | O_CREAT | O_APPEND, 0644);
    const char error[] = "Error opening file";
    if (fd == -1) {
        write(1, error, sizeof(error) - 1);
        return 1;
    }

    close(1);
    dup(fd);
    close(fd);

    char buffer[1024];

    while (1) {
        const ssize_t n = read(0, buffer, sizeof(buffer));
        if (n <= 0) {
            break;
        }

        write(1, buffer, n);
    }

    close(fd);

    return 0;
}
