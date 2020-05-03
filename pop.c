#include <err.h>
#include <stdio.h>

#define BUFSIZE 4096

FILE *open_score(FILE *fi, char *s) {
    fi = fopen(s, "r");
    if (fi == NULL) {
        errx(1, "problem opening score.");
    }
    return fi;
}

int main(int argc, char **argv) {
    char s[BUFSIZE] = {0};
    char *name = NULL;
    FILE *fi = NULL;

    if (argc != 2) {
        errx(1, "needs score file to read.");
    }
    name = argv[1];
    fi = open_score(fi, name);
    while (1) {
        fgets(s, BUFSIZE, stdin);
        if (fgets(s, BUFSIZE, fi) == NULL) {
            fclose(fi);
            fi = open_score(fi, name);
            fgets(s, BUFSIZE, fi);
        }
        printf("%s", s);
        fflush(stdout);
    }
    fclose(fi);
    return 0;
}
