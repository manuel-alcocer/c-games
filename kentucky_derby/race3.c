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

    while (gd->winner < 0){
        start_race (gd);
        if (gd->winner >= 0)
            sleep (2);
    }

    free (gd->horses_pos);
    free (gd->ahead_horses);
    exit (0);
}

int start_race (GD * gd) {
    advance_all_horses (gd);
    draw_screen (gd);
    advance_one_horse (gd);
    if (gd->tie_enabled)
        tiebreak (gd);
    draw_screen (gd);

    return gd->winner;
}
