#include "core.h"
#include "ncdraw.h"

int initncscreen (void) {
    initscr ();
    raw ();
    keypad (stdscr, TRUE);
    noecho ();
    curs_set (0);
    return 0;
}

NCWINS * init_ncwins (void) {
    NCWINS *ncw = (NCWINS *) malloc (sizeof (NCWINS));

    getmaxyx (stdscr, ncw->maxy, ncw->maxx);
    box (stdscr, 0, 0);
    refresh ();
    ncw->status_win = init_status_win (ncw);
    return ncw;
}

WINDOW * init_status_win (NCWINS * ncw) {
    WINDOW *local_win = newwin (6, ncw->maxx - 2, 1, 1);
    return local_win;
}

int ncdraw_screen (GD * gd, NCWINS *ncw) {
    clear_status_win (ncw);
    draw_status_win (gd, ncw);
    wait_for_secs (0, (1000 - SPEED * SPEED_FACTOR) * 1e6);
    return 0;
}

int clear_status_win (NCWINS *ncw) {
    return 0;
}

int draw_status_win (GD * gd, NCWINS *ncw) {
    char *lines[4];
    int lineslength[4];
    box (ncw->status_win, 0, 0);
    lineslength[0] =
        asprintf (&lines[0], "RACE NUM:%3d/%-3d", gd->race_num, gd->num_races);
    lineslength[1] =
        asprintf (&lines[1], "Horses num: %2d - Ahead horses num: %2d - Ahead pos: %3d - Winner: %2d - Track length: %3d - Tie: (%d) %3d)",
                gd->horses_num, gd->ahead_horses_num, gd->ahead_pos, gd->winner, gd->track_length, gd->tie_enabled, gd->tie_pos);
    asprintf (&lines[2], "Ahead horse list: [");
    for (int i = 0; i < gd->horses_num; i++)
        asprintf (&lines[2], "%s %3d", lines[2], gd->ahead_horses[i]);
    lineslength[2]
        = asprintf (&lines[2], "%s ] - Last Lucky horse: %2d", lines[2], gd->ahead_lucky_horse);
    asprintf (&lines[3], "      Horse list: [");
    for (int i = 0; i < gd->horses_num; i++)
        asprintf (&lines[3], "%s %3d", lines[3], gd->horses_pos[i]);
    lineslength[3] =
        asprintf (&lines[3], "%s ] - Last lucky horse: %2d", lines[3], gd->lucky_horse);
    for (int i = 0; i < 4; i++) {
        wmove (ncw->status_win, i, (ncw->maxx - 2 - lineslength[i]) / 2);
        wprintw (ncw->status_win, "%s", lines[i]);
    }
    wrefresh (ncw->status_win);
}
