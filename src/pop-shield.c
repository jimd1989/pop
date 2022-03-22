#include <err.h>
#include <stdio.h>

#define BUFSIZE 256

int main(int argc, char **argv) {
  char key = '\0';
  char cmd[BUFSIZE] = {0};
  if (argc != 2) { errx(1, "usage: <key char>"); }
  key = **++argv;
  while (1) {
    fgets(cmd, BUFSIZE, stdin);
    if (*cmd == key) { printf("%s", cmd + 1); fflush(stdout); }
  }
  return 0;
}
