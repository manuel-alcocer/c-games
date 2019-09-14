/*
 
 *      Kentucky Derby Simulator
 *     ==========================
 
 *  Original code by: manuel alcocer j. 

 *      <m.alcocer1978@gmail.com>

*/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define clearscr() printf("\033[H\033[J")

#define DEFHORSES 7
#define MINHORSES 2

#define TRACK_LENGTH 80
#define TIE_LIMIT 80

#define SPEED 97                // (L) 0 - (H) 99
#define SPEED_FACTOR 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define WINNER_COLOR ANSI_COLOR_GREEN

#define START_GRID "\u2520\u2500\u2500"
#define TRACK_PATH "\u2500"

typedef struct _horse {
    int horsenum;
    int * race_results;
} HORSE;

typedef struct _stats {
    int num_races;
    int num_horses;
    int track_length;
    HORSE * horselist;
} RACESTATS;

typedef struct _gamedata {
    int horses_num;
    int ahead_horses_num;
    int ahead_pos;
    int winner;
    int lucky_horse;
    int ahead_lucky_horse;
    int track_length;
    int tie_enabled;
    int * ahead_horses;
    int * horses_pos;
} GD;

int * advance_all_horses (GD * gd);

int * advance_one_horse (GD * gd);

int tiebreak (GD * gd);

int update_winner (GD * gd);

int update_ahead_pos (GD * gd);

GD * update_gd_vals (GD * gd);

int * update_ahead_horses (GD * gd);

void wait_for_secs (const int secs, const int nanosecs);

int gen_randnum(const int min, const int max);

int start_race (GD * gd);

void print_gd (GD * gd, int clr);

void print_race (GD * gd);

void draw (GD *gd);

GD * init_gamedata (GD * gd) {
    gd->horses_pos = (int *) calloc (gd->horses_num, sizeof (int));
    gd->ahead_horses = (int *) calloc (gd->horses_num, sizeof (int));
    gd->winner = -1;
    gd->lucky_horse = 0;
    gd->ahead_lucky_horse = 0;
    gd->ahead_horses_num = 0;
    gd->track_length = TRACK_LENGTH;
    gd->tie_enabled = 0;
    return gd;
}

int main (int argc, char **argv) {
    GD _gamedata, *gd = &_gamedata;

    if (argc < 2)
        gd->horses_num = DEFHORSES;
    else if (atoi(argv[1]) < MINHORSES)
        gd->horses_num = MINHORSES;
    else
        gd->horses_num = atoi(argv[1]);

    gd = init_gamedata (gd);

    while (gd->winner < 0){
        start_race (gd);
    }

    free (gd->horses_pos);
    free (gd->ahead_horses);
    exit (0);
}

int start_race (GD * gd) {
    advance_all_horses (gd);
    draw (gd);
    advance_one_horse (gd);
    if (gd->tie_enabled)
        tiebreak (gd);
    draw (gd);

    return gd->winner;
}

GD * update_gd_vals (GD * gd) {
    update_ahead_pos (gd);
    update_ahead_horses (gd);
    update_winner (gd);

    return gd;
}

int * advance_all_horses (GD * gd){
    for (int i = 0; i < gd->horses_num; i++)
        (gd->horses_pos[i])++;
    update_gd_vals (gd);

    return gd->horses_pos;
}

int tiebreak (GD * gd) {
    int c;
    if (gd->ahead_horses_num > 1){
        c = gen_randnum(0, gd->ahead_horses_num - 1);
        gd->ahead_lucky_horse = gd->ahead_horses[c];
        (gd->horses_pos[gd->ahead_lucky_horse])++;
        update_gd_vals (gd);
    }
    return gd->ahead_horses_num;
}

int * advance_one_horse (GD * gd) {
    gd->lucky_horse = gen_randnum(1, gd->horses_num);
    (gd->horses_pos[gd->lucky_horse])++;
    update_gd_vals (gd);

    return gd->horses_pos;
}

int update_ahead_pos (GD * gd) {
    gd->ahead_pos = 0;
    for (int i = 0; i < gd->horses_num; i++)
        if (gd->horses_pos[i] > gd->ahead_pos)
            gd->ahead_pos = gd->horses_pos[i];

    if (gd->ahead_pos > (gd->track_length * TIE_LIMIT) / 100 && gd->tie_enabled == 0)
        gd->tie_enabled = gd->ahead_pos;

    return gd->ahead_pos;
}

int * update_ahead_horses (GD * gd) {
    free (gd->ahead_horses);
    gd->ahead_horses = (int *) calloc (gd->horses_num, sizeof (int));
    gd->ahead_horses_num = 0;
    for (int i = 0; i < gd->horses_num; i++) {
        if (gd->horses_pos[i] == gd->ahead_pos) {
            ++(gd->ahead_horses_num);
            *(gd->ahead_horses + (gd->ahead_horses_num - 1)) = i;
        }
    }
    return gd->ahead_horses;
}

int update_winner (GD * gd) {
    for (int i = 0; i < gd->horses_num; i++)
        if (gd->horses_pos[i] == gd->track_length)
            gd->winner = i;
    return gd->winner;
}

void draw (GD *gd) {
    print_gd (gd, 1);
    print_race (gd);
    wait_for_secs (0, (1000 - SPEED * SPEED_FACTOR) * 1e6);
}

void print_color_line (GD * gd, const int horsenum) {
    if (gd->horses_pos[horsenum] == gd->ahead_pos)
        printf (WINNER_COLOR);
    else
        printf (ANSI_COLOR_RESET);

}

void print_start_line (const int pos, const int horsenum) {
    if (pos == 0)
        printf ("\t%2d" START_GRID, horsenum + 1);
}

void draw_line (GD * gd, const int horsenum) {
    for (int j = 0; j < gd->track_length; j++) {
        print_color_line (gd, horsenum);
        print_start_line (j, horsenum);
        if (j < gd->horses_pos[horsenum])
            printf ("\u2500%s", ANSI_COLOR_RESET);
    }
    printf ("\n");
}

void print_race (GD * gd) {
    printf ("\n\n");
    for (int i = 0; i < gd->horses_num; i++) {
        draw_line (gd, i);
    }
}

void print_gd (GD * gd, int clr) {
    if (clr)
        clearscr();
    printf (ANSI_COLOR_RED);
    printf ("\n  Horses num: %2d - Ahead horses num: %2d - Ahead pos: %3d - Winner: %2d - Track length: %3d - TIE: %d\n",
            gd->horses_num, gd->ahead_horses_num, gd->ahead_pos, gd->winner, gd->track_length, gd->tie_enabled);
    printf ("\n  \tAhead horse list: [");
    for (int i = 0; i < gd->horses_num; i++)
        printf ("%3d ", gd->ahead_horses[i]);
    printf ("] - Last Lucky horse: %2d\n", gd->ahead_lucky_horse);

    printf ("  \t      Horse list: [");
    for (int i = 0; i < gd->horses_num; i++)
        printf ("%3d ", gd->horses_pos[i]);
    printf ("] - Last lucky horse: %2d\n", gd->lucky_horse);
    printf (ANSI_COLOR_RESET);
}

void wait_for_secs (const int secs, const int nanosecs) {
    struct timespec trq, trm;

    trq.tv_sec = secs;
    trq.tv_nsec = nanosecs;
    nanosleep (&trq, &trm);
}

int gen_randnum(const int min, const int max){
    struct timeval t;

    gettimeofday (&t, NULL);
    srand (t.tv_sec * (int) 1e6 + t.tv_usec);

    return ((rand() % (max - min + 1)) + min);
}
