#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
#define LINESIZE 4096
#define MIN_ARGS 1
#define MAX_SCORES 32

typedef struct Scores {

/* Bounded master array of score files to switch between. Also holds current
 * command for parsing and dispatch. */

  int     ix;
  int     size;
  int     next;
  char    cmd[BUFSIZE];
  char    line[LINESIZE];
  FILE  * scores[MAX_SCORES];
} Scores;

FILE *openScore(char *arg) {

/* Turn an individual score file string into a file pointer. */

  FILE *fi = fopen(arg, "r");
  if (fi == NULL) { errx(1, "problem opening score %s", arg); }
  return fi;
}

Scores makeScores(int argc, char **argv) {

/* Parse argc and argv to make Scores. */

  Scores ss = {0};
  int i = 0;
  ss.size = --argc;
  if (argc < MIN_ARGS) { errx(1, "usage: <score files... >"); }
  if (argc > MAX_SCORES) { errx(1, "exceeded maximum %d scores.", MAX_SCORES); }
  for (; i < ss.size ; i++) { ss.scores[i] = openScore(argv[i]); }
  return ss;
}

void printLine(Scores *ss) {

/* Print one line from current score, looping to start if need be. */

  if (fgets(ss->line, LINESIZE, ss->scores[ss->ix]) == NULL) {
    rewind(ss->scores[ss->ix]);
    ss->ix = ss->next;
    ss->next = ss->ix;
    fgets(ss->line, LINESIZE, ss->scores[ss->ix]);
  }
  puts(ss->line);
  fflush(stdout);
}

void switchScore(Scores *ss) {

/* Read from a different score after current one finishes. */

  int ix = atoi(ss->cmd) - 1;
  if (ix < 0 || ix >= ss->size) {
    warnx("%d is not within range [0:%d]", ix + 1, ss->size);
    return;
  }
  ss->next = ix;
}


int main(int argc, char **argv) {
  Scores ss = makeScores(argc, argv);
  while (1) {
    fgets(ss.cmd, BUFSIZE, stdin);
    if (strlen(ss.cmd) == 0) { printLine(&ss); } else { switchScore(&ss); }
  }
    return 0;
}
