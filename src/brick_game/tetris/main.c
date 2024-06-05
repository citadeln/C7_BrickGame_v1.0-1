#include <stdio.h>
#include <stdlib.h>

#include "../../gui/cli/frontend.h"
#include "backend.h"

int main() {
  start_ncurses();
  game_loop();
  endwin();  // вернуть исходный вид терминала

  return 0;
}

void game_loop() {
  AllGame* all_game = createAllGame();
  TetPlayer player;  // структура для указания действий игрока
  player.action = TET_PLAYER_NOP;  // бездействует
  all_game->player = &player;

  make_start();

  dropNewFigure(all_game);

  // один игровой такт
  while (all_game->playing != TET_GAMEOVER) {
    all_game->player->action = get_action();
    userInput(all_game->player->action, 0);

    calculateTet(all_game);

    print_next_figure(all_game->tetg);
    printGameInfo_t(all_game);
    print_score_level_speed(all_game->tetg);

    sleep_milli(all_game->tetg->speed);
  }

  freeAllGame(all_game);
}

UserAction_t get_action() {
  UserAction_t action = TET_PLAYER_NOP;  // бездействует

  switch (getch()) {
    case '\n':
      action = Start;
      break;
    case ' ':
      action = Pause;
      break;
    case 'q':
      action = Terminate;
      break;
    case 'z':
      action = Action;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    default:
      action = TET_PLAYER_NOP;
      break;
  }

  return action;
}
