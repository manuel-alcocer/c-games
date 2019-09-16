#ifndef _DRAW_H
#define _DRAW_H

#include "core.h"

void print_gd (GD * gd, int clr);

void draw_race (GD * gd);

void draw_screen (GD *gd);

void draw_track (GD * gd, const int horsenum);

void draw_start_line (const int horsenum);

void draw_tie_line (const int pos, const int horsenum, GD * gd);

void draw_horse (void);

void draw_endline (const int horsenum, GD *gd);

void draw_horse_back (const int xpos, const int horsenum, GD * gd);

void draw_horse_front (const int xpos, const int horsenum, GD * gd);

char * choose_color_line (GD * gd, const int horsenum);

#endif
