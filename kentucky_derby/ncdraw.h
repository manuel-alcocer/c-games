#ifndef _NCDRAW_H
#define _NCDRAW_H

#include "core.h"
#include <ncurses.h>

#define MINWHEIGHT 2

#define STATUSLINES 4
#define STATSLINES 1

enum wins { MAINW, STATUSW, RACEW, STATSW };

typedef struct _win {
    WINDOW *win;
    int ypos;
    int xpos;
    int maxy;
    int maxx;
} WIN;

typedef struct _ncwins {
    int maxx;
    int maxy;
    WINDOW *main_win;
    WINDOW *status_win;
    WINDOW *race_win;
    WINDOW *stats_win;
} NCWINS;

WINDOW * create_win (int height, int width, int starty, int startx);

void destroy_win (WINDOW * window);
int get_stdscr_dims (NCWINS * ncw);

void redraw_all (NCWINS * ncw);

int calculate_dims (NCWINS *ncw, GD * gd);
int initncscreen (void);

NCWINS * init_ncwins (NCWINS * ncw, GD * gd);

WINDOW * init_status_win (NCWINS * ncw);

int ncdraw_screen (GD * gd, NCWINS *ncw);

int draw_status_win (GD * gd, NCWINS *ncw);

void create_main_win (NCWINS * ncw, GD * gd);

void create_status_win (NCWINS * ncw, GD * gd);

void create_race_win (NCWINS * ncw, GD * gd);

void create_stats_win (NCWINS * ncw, GD * gd);

#endif
