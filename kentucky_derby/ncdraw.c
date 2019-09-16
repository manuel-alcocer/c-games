#include "core.h"
#include "ncdraw.h"

int initncscreen (void) {
    initscr ();
    raw ();
    keypad (stdscr, TRUE);
    noecho ();
    curs_set (0);
}

NCWINS * ncinit_wins (void) {
    NCWINS *ncw = (NCWINS *) malloc (sizeof (NCWINS));
    ncw->status_win = (WINDOW *) malloc (sizeof (WINDOW));

    return ncw;
}

// MAIN Function for NCURSES
int ncdraw_screen (GD * gd, NCWINS *ncw) {
    ncdraw_gdwin (gd, ncw);
    wait_for_secs (0, (1000 - SPEED * SPEED_FACTOR) * 1e6);
    refresh();

}

int ncdraw_gdwin (GD * gd, NCWINS *ncw) {

}

void ncprint_gd (GD * gd) {
    move (0,0);
    printw ("  RACE NUM:%3d/%-3d"
            "\n  Horses num: %2d - Ahead horses num: %2d - Ahead pos: %3d - Winner: %2d - Track length: %3d - Tie: (%d) %3d)\n"
            "\n  \tAhead horse list: [",
                gd->race_num, gd->num_races, gd->horses_num, gd->ahead_horses_num, gd->ahead_pos, gd->winner,
                gd->track_length, gd->tie_enabled, gd->tie_pos);
    for (int i = 0; i < gd->horses_num; i++)
        printw ("%3d ", gd->ahead_horses[i]);
    printw ("] - Last Lucky horse: %2d\n"
            "  \t      Horse list: [",
                gd->ahead_lucky_horse);
    for (int i = 0; i < gd->horses_num; i++)
        printw ("%3d ", gd->horses_pos[i]);
    printw ("] - Last lucky horse: %2d\n", gd->lucky_horse);
}

