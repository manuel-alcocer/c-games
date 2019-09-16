#include "core.h"

#ifndef _NCDRAW_H
#define _NCDRAW_H
#include <ncurses.h>

typedef struct _ncwins {
    WINDOW *status_win;

} NCWINS;

int initncscreen (void);

int ncdraw_screen (GD * gd, NCWINS *ncw);

int ncdraw_gdwin (GD * gd, NCWINS *ncw);
void ncprint_gd (GD * gd);

NCWINS * ncinit_wins (void);

#endif
