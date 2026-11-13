#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int n;
  char buffer[4096];
  while ((n = read(0, buffer, 4096)) > 0) {
    write(1, buffer, n);
  }

  return 0;
}
