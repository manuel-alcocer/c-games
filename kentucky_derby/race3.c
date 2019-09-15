/*
 
 *      Kentucky Derby Simulator
 *     ==========================
 
 *  Original code by: manuel alcocer j. 

 *      <m.alcocer1978@gmail.com>

*/

#include "race.h"

int main (int argc, char **argv) {
    GD _gamedata, *gd = &_gamedata;

    if (argc < 2)
        gd->horses_num = DEFHORSES;
    else if (atoi(argv[1]) < MINHORSES)
        gd->horses_num = MINHORSES;
    else
        gd->horses_num = atoi(argv[1]);

    gd = init_gamedata (gd);

    run_game (gd);

    free (gd->horses_pos);
    free (gd->ahead_horses);
    exit (0);
}

int run_game (GD * gd) {
    int i = 0;
    while (i < gd->num_races){
        start_race (gd, i + 1);
        sleep (2);
        i++;
        gd = init_gamedata (gd);
    }
}

int start_race (GD * gd, const int race_num) {
    gd->race_num = race_num;
    while (gd->winner < 0){
        advance_all_horses (gd);
        draw_screen (gd);
        if (gd->winner >= 0)
            continue;
        advance_one_horse (gd);
        if (gd->winner >= 0)
            continue;
        if (gd->tie_enabled)
            tiebreak (gd);

        draw_screen (gd);
    }

    return gd->winner;
}
