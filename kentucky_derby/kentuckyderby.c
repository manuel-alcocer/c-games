/*
 
 *      Kentucky Derby Simulator
 *     ==========================
 
 *  Original code by: manuel alcocer j. 

 *      <m.alcocer1978@gmail.com>

*/

#include "core.h"

#ifndef TUI
#include "draw.h"
#else
#include "ncdraw.h"
#endif


int main (int argc, char **argv) {
    GD _gamedata, *gd = &_gamedata;
#ifdef TUI
    NCWINS *ncgamewins;
#endif


    if (argc < 2)
        gd->horses_num = DEFHORSES;
    else if (atoi(argv[1]) < MINHORSES)
        gd->horses_num = MINHORSES;
    else
        gd->horses_num = atoi(argv[1]);

    gd = init_gamedata (gd);

#ifndef TUI
    run_game (gd);
#else
    initncscreen ();
    ncgamewins = ncinit_wins ();
    run_game (gd, ncgamewins);
    free (ncgamewins->status_win);
    endwin ();
#endif

    free (gd->horses_pos);
    free (gd->ahead_horses);
    exit (0);
}

#ifndef TUI
int run_game (GD * gd) {
#else
int run_game (GD * gd, NCWINS *ncw) {
#endif
    int i = 0;

    while (i < gd->num_races){
#ifndef TUI
        start_race (gd, i + 1);
#else
        start_race (gd, i + 1, ncw);
#endif
        sleep (2);
        i++;
        gd = init_gamedata (gd);
    }
    return i;
}

#ifndef TUI
int start_race (GD * gd, const int race_num) {
#else
int start_race (GD * gd, const int race_num, NCWINS *ncgamewins) {
#endif
    gd->race_num = race_num;
    while (gd->winner < 0){
        advance_all_horses (gd);
#ifndef TUI
        draw_screen (gd);
#else
        ncdraw_screen (gd, ncgamewins);
#endif
        if (gd->winner >= 0)
            continue;
        advance_one_horse (gd);
#ifndef TUI
        draw_screen (gd);
#else
        ncdraw_screen (gd, ncgamewins);
#endif
        if (gd->winner >= 0)
            continue;
        if (gd->tie_enabled)
            if(tiebreak (gd))
#ifndef TUI
                draw_screen (gd);
#else
        ncdraw_screen (gd, ncgamewins);
#endif
    }

    return gd->winner;
}
