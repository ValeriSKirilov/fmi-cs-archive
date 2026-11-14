#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; i++) {
    int f = open(argv[i], O_RDONLY);

    if (f == -1) continue;

    int n;
    char buffer[4096];
    while ((n = read(f, buffer, 4096)) > 0) {
      write(1, buffer, n);
    }
  }
  return 0;
}
