
/* Code by: Manuel Alcocer J. <m.alcocer1978@gmail.com> */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define clearscr() printf("\033[H\033[J")

#define OPTSFIELD 2
#define AHEADPOS 1

#define DEFPLAYERS 7
#define MINPLAYERS 2
#define TRACK_LENGTH 40

#define TRACK_PATH "\u2500\u2500"
#define HORSE_DRAW "~\u256d\u2500\u256e^"
#define START_GRID "\u2520\u2500\u2500"
#define END_LINE "\u250b"
#define TOTAL_TRACK_LENGTH 3 + 1 + 3 + TRACK_LENGTH*2 + 2 + 5

#define SECS 0
#define NANOSECS 50 * 1e6

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int * horsegrid_create (const int num) {
    int * h = (int *) calloc (num + 2, sizeof (int));
    h[0] = num;

    return h;
}

void wait_for_secs (const int secs, const int nanosecs) {
    struct timespec trq, trm;

    trq.tv_sec = 0;
    trq.tv_nsec = nanosecs;
    nanosleep (&trq, &trm);
}

char * print_endline (const int curpos, const int total){
    char *lineprint = "";
    int i;
    for (i = curpos; i < total - 1; i++){
        asprintf (&lineprint, "%s ", lineprint);
        if (i == total - 1)
            asprintf (&lineprint, "%s%s", lineprint, END_LINE);
    }
    asprintf (&lineprint,"%s", lineprint);
    return lineprint;
}

void update_ahead_pos (int * horselist) {
    for (int i = 0; i < horselist[0]; i++)
        if (horselist[i+OPTSFIELD] >= horselist[AHEADPOS] - 2)
            horselist[AHEADPOS] = horselist[i+OPTSFIELD];
}

void draw_line (const int horsenum, int * horselist){
    char *lineprint = "", *endline;
    char *horsecolor;
    int plength = TOTAL_TRACK_LENGTH;

    update_ahead_pos(horselist);
    for (int j = 0; j < horselist[horsenum]; j++) {
        if (horselist[horsenum] > horselist[AHEADPOS])
            horsecolor = strdup(ANSI_COLOR_GREEN);
        else
            horsecolor = strdup(ANSI_COLOR_RESET);
        if (j == 0)
            asprintf (&lineprint, "%s\n%s%3d %s",
                    (endline = print_endline(0, plength)), horsecolor, horsenum-1, START_GRID);
        else if (j == horselist[horsenum] - 1)
            asprintf (&lineprint, "%s  %s%s%s",
                    lineprint, HORSE_DRAW, ANSI_COLOR_RESET, (endline = print_endline((j)*2 + 7 + 7, plength)));
        else
            asprintf (&lineprint, "%s%s%s", lineprint, horsecolor,TRACK_PATH);
    }

    if (horsenum > horselist[0])
        asprintf (&lineprint, "%s%s", lineprint, (endline = print_endline (0, plength)));

    printf ("%s\n", lineprint);
    free (lineprint);
    free (endline);
}

void write_status_line (int * horselist){
    printf (" Head current position: %3d\n", horselist[1]);
}

void paint_screen (int * horselist) {
    clearscr();

    printf ("\n");
    for (int i = 0; i < horselist[0]; i++){
        draw_line (i + OPTSFIELD, horselist);
    }

    write_status_line (horselist);

    wait_for_secs (SECS, NANOSECS);
}

int gen_randnum(const int min, const int max){
    struct timeval t;

    gettimeofday (&t, NULL);
    srand (t.tv_sec * (int) 1e6 + t.tv_usec);

    return ((rand() % (max - min + 1)) + min);
}

int winner_horse (int * horselist) {
    int winner = 0;

    for (int i = 0; i < horselist[0]; i++){
        if (horselist[i+OPTSFIELD] >= TRACK_LENGTH + 1)
            winner = horselist[i+OPTSFIELD];
    }

    return winner;
}

int advance_if_tie (int * horselist) {
    int *tied_horses = (int *) malloc (sizeof (int)), tied_num = 0, lucky_horse;

    for (int i = 0; i < horselist[0]; i++){
        if (horselist[i+OPTSFIELD] == horselist[AHEADPOS]){
            ++tied_num;
            tied_horses = (int *) realloc (tied_horses, tied_num * sizeof (int));
            tied_horses[i] = i + OPTSFIELD;
        }
    }

    lucky_horse = gen_randnum (0, tied_num - 1);

    if (tied_num)
        paint_screen (horselist);

    free (tied_horses);

    return tied_num;
}

int advance_one_horse (int * horselist) {
    int lucky_horse = gen_randnum (0, horselist[0]);

    ++horselist[lucky_horse + OPTSFIELD];

    paint_screen (horselist);

    return winner_horse (horselist);
}

int advance_all_horses (int * horselist) {
    for (int i = 0; i < horselist[0]; i++)
        ++horselist[i+OPTSFIELD];
    paint_screen (horselist);

    return winner_horse (horselist);
}

int main (int argc, char **argv) {
    int *horselist, horsenum = DEFPLAYERS, winner = 0;

    if (argc > 1)
       horsenum = atoi(argv[1]) < 2 ? MINPLAYERS : atoi(argv[1]);

    horselist = horsegrid_create (horsenum);
    while (! winner) {
        winner = advance_all_horses (horselist);
        if (! winner) {
            winner = advance_one_horse (horselist);
//            if (advance_if_tie (horselist))
//                paint_screen (horselist);
        }

    }

    free (horselist);

    exit (winner);
}

