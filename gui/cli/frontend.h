#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>
#include <time.h>

#include "./../../brick_game/tetris/backend.h"

void printGameInfo_t(AllGame* all_game);
void sleep_milli(int milliseconds);
void make_pause();
void make_start();
void start_ncurses();
void print_score_level_speed(GameInfo_t* tetg);
void print_next_figure(GameInfo_t* tetg);

#endif  // FRONTEND_H