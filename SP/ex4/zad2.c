//
// Created by Lenovo on 28.10.2026 г..
//

#include <fcntl.h>
#include <unistd.h>

int main() {
    const char msg[] = "EXAMPLE\n";
    for (int i = 0; i < 10; i++) {
        write(1, msg, sizeof(msg) - 1);
    }
}
