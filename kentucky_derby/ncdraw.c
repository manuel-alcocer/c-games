#include "core.h"
#include "ncdraw.h"

int initncscreen (void) {
    initscr ();
    raw ();
    keypad (stdscr, TRUE);
    noecho ();
    curs_set (0);
}

NCWINS * init_ncwins (void) {
    NCWINS *ncw = (NCWINS *) malloc (sizeof (NCWINS));

    getmaxyx (stdscr, ncw->maxy, ncw->maxx);
    box(stdscr, 0, 0);
    refresh();
    ncw->status_win = draw_status_win (ncw);

    return ncw;
}

WINDOW * draw_status_win (NCWINS * ncw) {
    WINDOW *local_win = newwin (6, ncw->maxx - 2, 1, 1);
    return local_win;
}

int ncdraw_screen (GD * gd, NCWINS *ncw) {
    clear_status_win (ncw);
    ncdraw_gdwin (gd, ncw);
    box (ncw->status_win, 0, 0);
    wrefresh(ncw->status_win);
    wait_for_secs (0, (1000 - SPEED * SPEED_FACTOR) * 1e6);

}

int clear_status_win (NCWINS *ncw) {
    
}

int ncdraw_gdwin (GD * gd, NCWINS *ncw) {
    ncprint_gd (gd, ncw);
}

void ncprint_gd (GD * gd, NCWINS * ncw) {
    wmove (ncw->status_win, 0,0);
    wprintw (ncw->status_win, "  RACE NUM:%3d/%-3d"
            "Horses num: %2d - Ahead horses num: %2d - Ahead pos: %3d - Winner: %2d - Track length: %3d - Tie: (%d) %3d)"
            "Ahead horse list: [",
                gd->race_num, gd->num_races, gd->horses_num, gd->ahead_horses_num, gd->ahead_pos, gd->winner,
                gd->track_length, gd->tie_enabled, gd->tie_pos);
    for (int i = 0; i < gd->horses_num; i++)
        wprintw (ncw->status_win, "%3d ", gd->ahead_horses[i]);
    wprintw (ncw->status_win, "] - Last Lucky horse: %2d\n"
            "Horse list: [",
                gd->ahead_lucky_horse);
    for (int i = 0; i < gd->horses_num; i++)
        wprintw (ncw->status_win, "%3d ", gd->horses_pos[i]);
    wprintw (ncw->status_win, "] - Last lucky horse: %2d\n", gd->lucky_horse);
}

