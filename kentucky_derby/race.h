#ifndef _RACE_H
#define _RACE_H

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

#define SPEED 97           // (L) 0 - (H) 99
#define SPEED_FACTOR 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define WINNER_COLOR ANSI_COLOR_GREEN

#define START_GRID "\u2520"
#define TRACK_PATH "\u2500"
#define HORSE_DRAW "~\u256d\u2500\u256e^"
#define TIE_LINE "\u250a"
#define END_LINE "\u250b"

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
    int tie_pos;
    int * ahead_horses;
    int * horses_pos;
} GD;

int start_race (GD * gd);

int * advance_all_horses (GD * gd);
int * advance_one_horse (GD * gd);

int tiebreak (GD * gd);

GD * init_gamedata (GD * gd);

GD * update_gd_vals (GD * gd);
int update_winner (GD * gd);
int update_ahead_pos (GD * gd);
int * update_ahead_horses (GD * gd);

void wait_for_secs (const int secs, const int nanosecs);
int gen_randnum(const int min, const int max);

void print_gd (GD * gd, int clr);
void draw_race (GD * gd);
void draw_screen (GD *gd);
void draw_track (GD * gd, const int horsenum);
void draw_start_line (const int horsenum);
void draw_tie_line (const int pos, const int horsenum, GD * gd);
void draw_horse (void);
void draw_endline (void);
void draw_horse_back (const int xpos, const int horsenum, GD * gd);
void draw_horse_front (const int xpos, const int horsenum, GD * gd);
char * choose_color_line (GD * gd, const int horsenum);

#endif
