//
// Created by Lenovo on 21.10.2026 г..
//

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int main(const int argc, char *argv[]) {
    const char arguments_error[] = "Invalid number of arguments. A number and a filename are required.\n";
    if (argc < 3) {
        write(1, arguments_error, sizeof(arguments_error) - 1);
        return 1;
    }

    const int fd = open(argv[2], O_RDONLY);
    if (fd == -1) {
        const char opening_file_error[] = "Error opening file\n";
        write(1, opening_file_error, sizeof(opening_file_error) - 1);
        return 1;
    }

    int result = 0;
    for (int i = 0; argv[1][i] != '\0'; i++) {
        result = result * 10 + (argv[1][i] - '0');
    }

    char buffer[1024];
    int reachedRow = 0;
    int rowsPassed = 0;
    ssize_t n;

    if (result <= 1) {
        reachedRow = 1;
    }
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        printf("%zd\n", n);
        write(1, buffer, n);
        if (reachedRow) {
            write(1, buffer, n);
        } else {
            int pos = 0;
            char tempBuffer[1024];
            for (int i = 0; i < n; i++) {
                if (reachedRow) {
                    tempBuffer[pos++] = buffer[i];
                    continue;
                }
                if (buffer[i] == '\n') {
                    rowsPassed++;
                }
                if (rowsPassed == result - 1) {
                    reachedRow = 1;
                }
            }
            if (reachedRow)
                write(1, tempBuffer, pos);
        }
    }

    close(fd);

    return 0;
}
