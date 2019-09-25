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

NCWINS * init_ncwins (NCWINS * ncw, GD * gd) {
    get_stdscr_dims (ncw);
    create_main_win (ncw, gd);
    create_status_win (ncw, gd);
    create_race_win (ncw, gd);
    create_stats_win (ncw, gd);
    return ncw;
}

void create_main_win (NCWINS * ncw, GD * gd) {
    int wh =
        3 * MINWHEIGHT
        + (gd->horses_num * 2 + 1)
        + STATUSLINES + STATSLINES;
    int ww = ncw->maxx;
    ncw->main_win = create_win (wh, ww, 0, 0); 
    wrefresh (ncw->main_win);
}

void create_status_win (NCWINS * ncw, GD * gd) {
    ncw->status_win = create_win (6, ncw->maxx - 2, 1, 1);
}

void create_race_win (NCWINS * ncw, GD * gd) {
    ncw->race_win = create_win (gd->horses_num * 2 + 1, ncw->maxx - 2, 7, 1);
    wrefresh (ncw->race_win);
}

void create_stats_win (NCWINS * ncw, GD * gd) {
    ncw->stats_win = create_win (3, ncw->maxx - 2, gd->horses_num * 2 + 1 + 6 + 1, 1);
    wrefresh (ncw->stats_win);
}

int ncdraw_screen (GD * gd, NCWINS *ncw) {
    draw_status_win (gd, ncw);
    wait_for_secs (0, (1000 - SPEED * SPEED_FACTOR) * 1e6);
    return 0;
}

int draw_status_win (GD * gd, NCWINS *ncw) {
    char *lines[4];
    int lineslength[4];
    box (ncw->status_win, 0, 0);
    lineslength[0] =
        asprintf (&lines[0], "RACE NUM: %d/%d", gd->race_num, gd->num_races);
    lineslength[1] =
        asprintf (&lines[1], "Horses num: %d - Ahead horses num: %d - Ahead pos: %d - Winner: %d - Track length: %d - Tie: (%d) %d",
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
        wmove (ncw->status_win, i + 1, (ncw->maxx - 2 - lineslength[i]) / 2);
        wprintw (ncw->status_win, "%s", lines[i]);
    }
    wrefresh (ncw->status_win);
    return 0;
}

WINDOW * create_win (int h, int w, int sy, int sx) {
    WINDOW *local_win = newwin (h, w, sy, sx);
    box (local_win, 0, 0);

    return local_win;
}

void destroy_win (WINDOW * win) {
    wborder (win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh (win);
    delwin (win);
}

int get_stdscr_dims (NCWINS * ncw) {
    getmaxyx (stdscr, ncw->maxy, ncw->maxx);

    return 0;
}

void redraw_all (NCWINS * ncw) {

}

int calculate_dims (NCWINS *ncw, GD * gd) {
}
