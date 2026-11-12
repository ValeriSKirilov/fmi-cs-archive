//
// Created by Lenovo on 4.11.2026 г..
//

#include <fcntl.h>
#include <unistd.h>
#include <bits/fcntl-linux.h>
#include <sys/wait.h>

int main(const int argc, char *argv[]) {
    const char no_file_error[] = "No filename provided\n";
    if (argc < 2) {
        write(1, no_file_error, sizeof(no_file_error) - 1);
        return 1;
    }

    const int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    const char open_file_error[] = "Error opening file\n";
    if (fd == -1) {
        write(1, open_file_error, sizeof(open_file_error) - 1);
        return 1;
    }



    return 0;
}
