#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define clearscr() printf("\033[H\033[J")

#define HORSE_NUM 7
#define HORSES_MAX 15
#define TRACK_LENGTH 40
#define nanosecs 90000 * 1e3

int gen_rand(int min, int max){
    struct timeval t;
    gettimeofday (&t, NULL);
    srand (t.tv_sec * (int) 1e6 + t.tv_usec);
    return ((rand() % (max - min + 1)) + min);
}

int * horsegrid_create (int horse_num) {
    int *horsegrid_list = (int *) calloc (horse_num + 1, sizeof (int));
    horsegrid_list[0] = horse_num;
    return horsegrid_list;
}

void print_horses_position (int *horse_list) {
    clearscr();
    for (int y = 1; y < horse_list[0] + 1; y++){
        for (int x = 0; x < horse_list[y]; x++){
            if (x == 0) printf ("%d ", y);
            printf("- ");
        }
        printf ("\n");
    }
}

void advance_all_horses (int *horse_list) {
    for (int i=0; i < *horse_list; i++)
        ++(*(horse_list + i + 1));
    print_horses_position(horse_list);
}

void advance_one_horses (int *horse_list) {
    int lucky_horse = gen_rand(1, horse_list[0]);
    *(horse_list + lucky_horse) = *(horse_list + lucky_horse) + 1;
    print_horses_position(horse_list);
}
int check_horses_pos (int *horse_list) {
    int winner_horse = 0;
    for (int i=0; i < horse_list[0] + 1; i++)
        if (horse_list[i + 1] >= TRACK_LENGTH)
            return horse_list[i + 1];
    return winner_horse;
}

int update_horses_position (int *horse_list) {
    int i = 0, winner_horse = 0;
    struct timespec trq, trm;
    trq.tv_sec = 0;
    trq.tv_nsec = nanosecs;
    while (i < 2 && ! winner_horse)
    {
        advance_one_horses (horse_list);
        nanosleep (&trq, &trm);
        if (! (winner_horse = check_horses_pos(horse_list))){
            advance_all_horses (horse_list);
            nanosleep (&trq, &trm);
        }
    }

    return 1;
}

int main (int argc, char * *argv) {
    int *horse_list, horse_num, winner_horse = 0;

    horse_num = argc < 2? HORSE_NUM : atoi (argv[1]);
    if ( horse_num > HORSES_MAX)
        horse_num = HORSES_MAX;

    horse_list = horsegrid_create (horse_num);
    advance_all_horses (horse_list);
    while (! winner_horse) {
        winner_horse = update_horses_position (horse_list);
    }
    exit (winner_horse);
}

