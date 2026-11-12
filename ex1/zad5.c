//
// Created by Lenovo on 20.10.2026 г..
//

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        const char arguments_error[] = "Invalid number of arguments. A filename is required.\n";
        write(2, arguments_error, sizeof(arguments_error) - 1);
        return 1;
    }

    if (argc == 2 && argv[1][0] == '-') {
        const char no_file_error[] = "At least one file is required.\n";
        write(2, no_file_error, sizeof(no_file_error) - 1);
        return 1;
    }

    int fds[argc];
    int totalAdded = 0;

    int count_c = 0, count_w = 0, count_l = 0;
    if (argv[1][0] != '-') {
        count_c = count_w = count_l = 1;
    }

    const char opening_file_error[] = "Error opening file.\n";
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'c':
                        count_c = 1;
                        break;
                    case 'w':
                        count_w = 1;
                        break;
                    case 'l':
                        count_l = 1;
                        break;
                    default:
                        break;
                }
            }
            continue;
        }

        const int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            write(2, opening_file_error, sizeof(opening_file_error) - 1);
        } else {
            fds[totalAdded++] = fd;
        }
    }

    for (int i = 0; i < totalAdded; i++) {
        ssize_t chars = 0, words = 0, lines = 0;
        char buffer[1024];
        ssize_t n;
        int in_word = 0;

        while ((n = read(fds[i], buffer, sizeof(buffer))) > 0) {
            chars += n;
            for (int j = 0; j < n; j++) {
                if (buffer[j] == '\n') {
                    in_word = 0;
                    lines++;
                    continue;
                }
                if (buffer[j] == ' ' || buffer[j] == '\t') {
                    in_word = 0;
                    continue;
                }
                if (in_word == 0) {
                    words++;
                    in_word = 1;
                }
            }
        }

        if (count_l) {
            printf("%zd ", lines);
        }
        if (count_w) {
            printf("%zd ", words);
        }
        if (count_c) {
            printf("%zd ", chars);
        }
        write(1, "\n", 1);
    }

    for (int i = 0; i < totalAdded; i++) {
        close(fds[i]);
    }

    return 0;
}
