#include "race.h"

GD * init_gamedata (GD * gd) {
    gd->horses_pos = (int *) calloc (gd->horses_num, sizeof (int));
    gd->ahead_horses = (int *) calloc (gd->horses_num, sizeof (int));
    gd->winner = -1;
    gd->lucky_horse = 0;
    gd->ahead_lucky_horse = 0;
    gd->ahead_horses_num = 0;
    gd->track_length = TRACK_LENGTH;
    gd->tie_enabled = 0;
    gd->tie_pos = (gd->track_length * TIE_LIMIT) / 100;
    gd->scrtln = TRACK_LENGTH * TRACK_PATH_WIDTH + HORSE_DRAW_WIDTH + END_LINE_WIDTH;
    gd->num_races = DEFRACES;
    gd->race_num = 0;
    return gd;
}

GD * update_gd_vals (GD * gd) {
    update_ahead_pos (gd);
    update_ahead_horses (gd);
    update_winner (gd);

    return gd;
}

int advance_all_horses (GD * gd){
    for (int i = 0; i < gd->horses_num; i++)
        (gd->horses_pos[i])++;
    update_gd_vals (gd);

    return gd->winner;
}

int tiebreak (GD * gd) {
    int c;
    if (gd->ahead_horses_num > 1){
        c = gen_randnum(0, gd->ahead_horses_num - 1);
        gd->ahead_lucky_horse = gd->ahead_horses[c];
        (gd->horses_pos[gd->ahead_lucky_horse])++;
        update_gd_vals (gd);
        return 1;
    }
    return 0;
}

int advance_one_horse (GD * gd) {
    gd->lucky_horse = gen_randnum(1, gd->horses_num);
    (gd->horses_pos[gd->lucky_horse])++;
    update_gd_vals (gd);

    return gd->winner;
}

int update_ahead_pos (GD * gd) {
    gd->ahead_pos = 0;
    for (int i = 0; i < gd->horses_num; i++)
        if (gd->horses_pos[i] > gd->ahead_pos)
            gd->ahead_pos = gd->horses_pos[i];

    if (gd->ahead_pos > gd->tie_pos)
        gd->tie_enabled = 1;

    return gd->ahead_pos;
}

int * update_ahead_horses (GD * gd) {
    free (gd->ahead_horses);
    gd->ahead_horses = (int *) calloc (gd->horses_num, sizeof (int));
    gd->ahead_horses_num = 0;
    for (int i = 0; i < gd->horses_num; i++) {
        if (gd->horses_pos[i] == gd->ahead_pos) {
            ++(gd->ahead_horses_num);
            *(gd->ahead_horses + (gd->ahead_horses_num - 1)) = i;
        }
    }
    return gd->ahead_horses;
}

int update_winner (GD * gd) {
    for (int i = 0; i < gd->horses_num; i++)
        if (gd->horses_pos[i] == gd->track_length)
            gd->winner = i;
    return gd->winner;
}

void wait_for_secs (const int secs, const int nanosecs) {
    struct timespec trq, trm;

    trq.tv_sec = secs;
    trq.tv_nsec = nanosecs;
    nanosleep (&trq, &trm);
}

int gen_randnum(const int min, const int max){
    struct timeval t;

    gettimeofday (&t, NULL);
    srand (t.tv_sec * (int) 1e6 + t.tv_usec);

    return ((rand() % (max - min + 1)) + min);
}

