#include "../brick_game/tetris/backend.h"
#include "test.h"

static int array1[] = {
    0, 1, 7, 6, 3, 4, 2,
};
// static int array2[] = {15, 19, 3, 8, 6, 1, -1, 0};
static int array3[] = {1, 2, 3, 4, 5, 6, 7};
static int array4[] = {15, 16, 17, 18, 19};

/* GCOV_calculateTet_test_START */
START_TEST(my_calculateTet_test) {
  AllGame *all_game = createAllGame();
  TetPlayer player;  // структура для указания действий игрока
  player.action = TET_PLAYER_NOP;  // бездействует
  all_game->player = &player;
  dropNextNewFigure(all_game);
  all_game->player->action = array1[_i];
  calculateTet(all_game);
  freeAllGame(all_game);
}

START_TEST(collision_test) {
  AllGame *all_game = createAllGame();
  dropNewFigure(all_game);
  all_game->figure->y = 2;

  for (int i = array3[_i]; i < MY_FIELD_HIGHT; i++)
    for (int j = 0; j < MY_FIELD_WIDTH; j++) all_game->tetg->field[i][j] = 1;
  if (collisionTet(all_game)) {
    moveFigureUp(all_game);
    plantFigure(all_game);  // переносим её блоки на поле

    // считаем количество строк заполненных блоками, удалить эти строки,
    // добавить их кол-во к числу очков в игре
    all_game->tetg->score += eraseLinesTet(all_game->tetg);
    change_speed(all_game->tetg);
  }

  freeAllGame(all_game);
}

START_TEST(collision_test2) {
  AllGame *all_game = createAllGame();
  dropNewFigure(all_game);
  all_game->figure->y = 2;
  all_game->ticks_left = array3[_i] + 1;
  for (int i = array3[_i]; i < MY_FIELD_HIGHT; i++)
    for (int j = 0; j < MY_FIELD_WIDTH; j++) all_game->tetg->field[i][j] = 1;
  if (collisionTet(all_game)) {
    moveFigureUp(all_game);
    plantFigure(all_game);  // переносим её блоки на поле

    // считаем количество строк заполненных блоками, удалить эти строки,
    // добавить их кол-во к числу очков в игре
    all_game->tetg->score += eraseLinesTet(all_game->tetg);
    change_speed(all_game->tetg);
  }
  freeAllGame(all_game);
}

START_TEST(lines_test) {
  userInput(array1[_i], 0);

  AllGame *all_game = createAllGame();

  for (int i = array4[_i]; i < MY_FIELD_HIGHT; i++)
    for (int j = 0; j < MY_FIELD_WIDTH; j++) all_game->tetg->field[i][j] = 1;
  int count = eraseLinesTet(all_game->tetg);

  dropLineTet(0 * count, all_game->tetg->field);

  freeAllGame(all_game);
}

Suite *calculateTet_test(void) {
  Suite *s = suite_create("calculateTet_test");
  TCase *tc = tcase_create("calculateTet_test");
  tcase_add_loop_test(tc, my_calculateTet_test, 0,
                      sizeof(array1) / sizeof(int));
  tcase_add_loop_test(tc, collision_test, 0, sizeof(array1) / sizeof(int));
  tcase_add_loop_test(tc, collision_test2, 0, sizeof(array1) / sizeof(int));
  tcase_add_loop_test(tc, lines_test, 0, sizeof(array4) / sizeof(int));

  suite_add_tcase(s, tc);
  return s;
}
