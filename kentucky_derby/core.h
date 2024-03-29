#ifndef _CORE_H
#define _CORE_H

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

#ifndef TUI
#define DEFRACES 10
#else
#define DEFRACES 1
#endif

#define DEFHORSES 7
#define MINHORSES 2

#define TRACK_LENGTH 80
#define TIE_LIMIT 80

#define SPEED 97           // (L) 0 - (H) 99
#define SPEED_FACTOR 10

#define ANSI_COLOR_RED     "\x1b[91m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define WINNER_COLOR ANSI_COLOR_GREEN

#define HORSENUM_WIDTH 2

#define BEGIN_OFFSET "        "
#define BEGIN_OFFSET_WIDTH 8

#define START_GRID " \u2520"
#define START_GRID_WIDTH 2

#define TRACK_PATH "\u2500"
#define TRACK_PATH_WIDTH 1

#define HORSE_DRAW " ~\u256d\u2500\u256e^"
#define HORSE_DRAW_WIDTH 6

#define TIE_LINE "\u250a"
#define TIE_LINE_WIDTH 1

#define END_LINE "\u250b"
#define END_LINE_WIDTH 1

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
    int num_races;
    int race_num;
    int horses_num;
    int ahead_horses_num;
    int ahead_pos;
    int winner;
    int lucky_horse;
    int ahead_lucky_horse;
    int track_length;
    int tie_enabled;
    int tie_pos;
    int scrtln;
    int * ahead_horses;
    int * horses_pos;
} GD;

#ifdef TUI
#include "ncdraw.h"
#endif

#ifndef TUI
int start_race (GD * gd, const int race_num);
int run_game (GD * gd);
#else
int start_race (GD * gd, const int race_num, NCWINS *ncgamewins);
int run_game (GD * gd, NCWINS *ncw);
#endif

int advance_all_horses (GD * gd);
int advance_one_horse (GD * gd);
int tiebreak (GD * gd);

GD * init_gamedata (GD * gd);

GD * update_gd_vals (GD * gd);
int update_winner (GD * gd);
int update_ahead_pos (GD * gd);
int * update_ahead_horses (GD * gd);

void wait_for_secs (const int secs, const int nanosecs);
int gen_randnum(const int min, const int max);

#endif
