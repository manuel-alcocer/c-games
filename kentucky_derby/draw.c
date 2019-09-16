#include "core.h"
#include "draw.h"

void draw_screen (GD *gd) {
    print_gd (gd, 1);
    draw_race (gd);
    wait_for_secs (0, (1000 - SPEED * SPEED_FACTOR) * 1e6);
}

void draw_race (GD * gd) {
    printf ("\n\n");
    for (int i = 0; i < gd->horses_num; i++) {
        draw_track (gd, i);
    }
}

void draw_track (GD * gd, const int horsenum) {
    for (int xpos = 0; xpos < gd->track_length; xpos++) {
        if (xpos < gd->horses_pos[horsenum] - 1)
            draw_horse_back (xpos, horsenum, gd);
        else if (xpos == gd->horses_pos[horsenum] - 1)
            draw_horse ();
        else
            draw_horse_front (xpos, horsenum, gd);
    }
    draw_endline (horsenum, gd);
    printf ("\n");
}

void draw_horse_back (const int xpos, const int horsenum, GD * gd) {
    if (xpos == 0) {
        printf ("\t");
        draw_start_line (horsenum);
    }
    else if (xpos != gd->tie_pos)
        printf ("%s" TRACK_PATH ANSI_COLOR_RESET, choose_color_line (gd, horsenum));
    else if (xpos == gd->tie_pos)
        printf (TIE_LINE);
}

void draw_horse_front (const int xpos, const int horsenum, GD * gd) {
    if (xpos < gd->track_length - 1)
        printf (" ");
}

void draw_horse (void) {
    printf (HORSE_DRAW);
}

void draw_endline (const int horsenum, GD * gd) {
    if (gd->horses_pos[horsenum] < gd->track_length)
        printf (END_LINE);
}

char * choose_color_line (GD * gd, const int horsenum) {
    if (gd->horses_pos[horsenum] == gd->ahead_pos)
        return WINNER_COLOR;
    else
        return ANSI_COLOR_RESET;
}

void draw_start_line (const int horsenum) {
    printf (ANSI_COLOR_RESET);
    printf ("%2d " START_GRID, horsenum + 1);
}

void draw_blankline (GD * gd) {
    int init_offset = 2 + 1 + 1;
    int end_offset = init_offset + 7;
    printf (ANSI_COLOR_RESET);
    printf ("\t");
    for (int xpos = 0; xpos < gd->track_length + end_offset; xpos++) {
        if (xpos == gd->track_length - 1 + end_offset)
            printf (END_LINE);
        else
            printf (" ");
    }
    printf ("\n");
}

void print_gd (GD * gd, int clr) {
    if (clr)
        clearscr();
    printf (ANSI_COLOR_RED);
    printf ("  RACE NUM:%3d/%-3d"
            "\n  Horses num: %2d - Ahead horses num: %2d - Ahead pos: %3d - Winner: %2d - Track length: %3d - Tie: (%d) %3d)\n"
            "\n  \tAhead horse list: [",
                gd->race_num, gd->num_races, gd->horses_num, gd->ahead_horses_num, gd->ahead_pos, gd->winner,
                gd->track_length, gd->tie_enabled, gd->tie_pos);
    for (int i = 0; i < gd->horses_num; i++)
        printf ("%3d ", gd->ahead_horses[i]);
    printf ("] - Last Lucky horse: %2d\n"
            "  \t      Horse list: [",
                gd->ahead_lucky_horse);
    for (int i = 0; i < gd->horses_num; i++)
        printf ("%3d ", gd->horses_pos[i]);
    printf ("] - Last lucky horse: %2d\n", gd->lucky_horse);
    printf (ANSI_COLOR_RESET);
}

