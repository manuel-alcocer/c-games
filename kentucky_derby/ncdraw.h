#include "core.h"

#ifndef _NCDRAW_H
#define _NCDRAW_H
#include <ncurses.h>

typedef struct _ncwins {
    int maxx;
    int maxy;
    WINDOW *status_win;

} NCWINS;

int initncscreen (void);

NCWINS * init_ncwins (void);

WINDOW * draw_status_win (NCWINS * ncw);

int ncdraw_screen (GD * gd, NCWINS *ncw);

int ncdraw_gdwin (GD * gd, NCWINS *ncw);
void ncprint_gd (GD * gd, NCWINS *ncw);


#endif
