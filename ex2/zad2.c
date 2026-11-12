//
// Created by Lenovo on 14.10.2026 г..
//

//
// Created by Lenovo on 14.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    const int fd = open("ex2/aa", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    char buffer[16];

    for (int i = 5; i >= 1; i--) {
        const ssize_t c = read(fd, buffer, i);
        write(1, buffer, c);
        write(1, "\n", 1);
    }

    close(fd);

    return 0;
}
