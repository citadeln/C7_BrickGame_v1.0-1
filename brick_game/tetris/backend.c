/**
 * @file
 * @brief Основные функции работы игры.
 */

#include "backend.h"

#include <stdlib.h>

#ifndef TETRIS_C
#define TETRIS_C

AllGame* createAllGame() {
  AllGame* all_game = (AllGame*)malloc(sizeof(AllGame));

  all_game->tetg = get_field_info(all_game);
  all_game->tet_templates = createTemplates();
  all_game->figure = createTetFigure();
  all_game->ticks = TET_TICKS_START;
  all_game->ticks_left = TET_TICKS_START;
  all_game->playing = TET_PLAYING;

  return all_game;  // указатель на инициированную основную структуру GameInfo_t
}

void freeAllGame(AllGame* all_game) {
  if (all_game) {
    freeGameInfo_t(all_game->tetg);
    freeTetFigure(all_game->figure);
    // if (all_game->tet_templates) free(all_game->tet_templates);
    free(all_game);
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t* gameinfo = get_field_info(NULL);

  return *gameinfo;
}

GameInfo_t* get_field_info(AllGame* all_game) {
  static GameInfo_t* tetg_info = NULL;

  if (all_game != NULL) tetg_info = createGameInfo_t();

  return tetg_info;
}

GameInfo_t* createGameInfo_t() {
  GameInfo_t* tetg = (GameInfo_t*)malloc(sizeof(GameInfo_t));

  tetg->field = createTetField();
  tetg->next = createNext();

  tetg->score = 0;
  tetg->high_score = read_high_score();
  tetg->level = 1;
  tetg->speed = VERYSLOWSPEED;
  tetg->pause = 0;  // нет паузы

  return tetg;
}

void freeGameInfo_t(GameInfo_t* tetg) {
  if (tetg) {
    freeTetField(tetg->field);
    freeNext(tetg->next);
    free(tetg);
  }
}

int** createNext() {
  int** matrix = (int**)calloc(MY_FIGURES_SIZE, sizeof(int*));
  for (int i = 0; i < MY_FIGURES_SIZE; i++)
    matrix[i] = (int*)calloc(MY_FIGURES_SIZE, sizeof(int));

  return matrix;
}

void freeNext(int** next) {
  if (next) {
    for (int i = 0; i < MY_FIGURES_SIZE; i++) {
      for (int j = 0; j < MY_FIGURES_SIZE; j++) next[i][j] = 0;
      free(next[i]);
    }
    free(next);
  }
}

int** createTetField() {
  int** matrix = (int**)calloc(MY_FIELD_HIGHT, sizeof(int*));
  for (int i = 0; i < MY_FIELD_HIGHT; i++)
    matrix[i] = (int*)calloc(MY_FIELD_WIDTH, sizeof(int));

  return matrix;
}

void freeTetField(int** tetf) {
  if (tetf) {
    for (int i = 0; i < MY_FIELD_HIGHT; i++) {
      for (int j = 0; j < MY_FIELD_WIDTH; j++) tetf[i][j] = 0;
      free(tetf[i]);
    }
    free(tetf);
  }
}

/**
 * @brief создание и инициализация фигуры
 */
TetFigure* createTetFigure() {
  TetFigure* t = (TetFigure*)malloc(sizeof(TetFigure));
  t->x = 0;
  t->y = 0;

  for (int i = 0; i < MY_FIGURES_SIZE * MY_FIGURES_SIZE; i++) t->blocks[i] = 0;

  return t;
}

void freeTetFigure(TetFigure* tf) {
  if (tf) free(tf);
}

int* createTemplates() {
  static int temp[MY_FIGURES_SIZE * MY_FIGURES_SIZE * COUNT_OF_FIGUREST] = {
      // 0, 1, 1, 0,  //
      // 0, 1, 1, 0,  //
      // 0, 1, 1, 0,  //
      // 0, 1, 1, 0,  //

      0, 1, 0, 0,  //
      0, 1, 0, 0,  //
      0, 1, 0, 0,  //
      0, 1, 0, 0,  //

      0, 1, 0, 0,  //
      1, 1, 1, 0,  //
      0, 0, 0, 0,  //
      0, 0, 0, 0,  //

      0, 1, 0, 0,  //
      0, 1, 1, 1,  //
      0, 0, 0, 0,  //
      0, 0, 0, 0,

      0, 0, 0, 1,  //
      0, 1, 1, 1,  //
      0, 0, 0, 0,  //
      0, 0, 0, 0,  //

      0, 0, 0, 0,  //
      0, 0, 1, 1,  //
      0, 1, 1, 0,  //
      0, 0, 0, 0,  //

      0, 0, 0, 0,  //
      0, 1, 1, 0,  //
      0, 0, 1, 1,  //
      0, 0, 0, 0,  //

      0, 0, 0, 0,  //
      0, 1, 1, 0,  //
      0, 1, 1, 0,  //
      0, 0, 0, 0,  //
  };

  static int* tet_templates = temp;

  return tet_templates;
}

/**
 * @brief расчет такта одного игрового цикла
 */
void calculateTet(AllGame* all_game) {
  GameInfo_t* tetg = all_game->tetg;
  if (all_game->ticks_left <= 0) {
    all_game->ticks_left = all_game->ticks;

    moveFigureDown(all_game);  // передвигаем фигуру на один блок вниз
    if (collisionTet(all_game)) {
      moveFigureUp(all_game);
      plantFigure(all_game);  // переносим её блоки на поле

      // считаем количество строк заполненных блоками, удалить эти строки,
      // добавить их кол-во к числу очков в игре
      tetg->score += eraseLinesTet(tetg);
      change_speed(tetg);
      if (tetg->high_score <= tetg->score) input_high_score(tetg->score);

      dropNextNewFigure(all_game);  // выбрасываем фигуру на поле
      if (collisionTet(all_game)) {
        all_game->playing = TET_GAMEOVER;
        return;
      }
    }
  }

  // обработка действий игрока
  switch (all_game->player->action) {
    case Right:
      moveFigureRight(all_game);
      if (collisionTet(all_game)) moveFigureLeft(all_game);
      break;
    case Left:
      moveFigureLeft(all_game);
      if (collisionTet(all_game)) moveFigureRight(all_game);
      break;
    case Down:
      moveFigureDown(all_game);
      if (collisionTet(all_game)) moveFigureUp(all_game);
      break;
    case Action: {  // фигура будет повернута
      TetFigure* t = rotTetFigure(
          all_game);  // переменная для хранения фигуры после поворота
      TetFigure* told = all_game->figure;  // текущая версия
      all_game->figure = t;
      if (collisionTet(all_game)) {
        all_game->figure = told;
        freeTetFigure(t);
      } else
        freeTetFigure(told);

      // Я ДУМАЮ НАДО ОЧИСТИТЬ ОБЕ И TOLD B T!!!!!!!! ПЕРЕПИСАТЬ!!!
    } break;
    case Pause:
      all_game->tetg->pause = (all_game->tetg->pause) ? 0 : 1;
      break;
    case Terminate:
      all_game->playing = TET_GAMEOVER;
      break;
    case Start:
      break;
  };

  all_game->ticks_left--;
}

void moveFigureDown(AllGame* all_game) { all_game->figure->y++; }

void moveFigureUp(AllGame* all_game) { all_game->figure->y--; }

void moveFigureRight(AllGame* all_game) { all_game->figure->x++; }

void moveFigureLeft(AllGame* all_game) { all_game->figure->x--; }

int collisionTet(AllGame* all_game) {
  int temp = 0;
  TetFigure* t = all_game->figure;
  for (int i = 0; i < MY_FIGURES_SIZE; i++)      // i это y
    for (int j = 0; j < MY_FIGURES_SIZE; j++) {  // j это x
      if (t->blocks[MY_FIGURES_SIZE * i + j] != 0) {
        int fx = t->x + j;
        int fy = t->y + i;
        if (fx < 0 || fx >= MY_FIELD_WIDTH || fy < 0 || fy >= MY_FIELD_HIGHT)
          temp = 1;
        else if (all_game->tetg->field[fy][fx] != 0)
          temp = 1;
      }
    }
  return temp;
}

/**
 * @brief размещение фигуры после падения
 * @param all_game Указатель на AllGame*.
 */
void plantFigure(AllGame* all_game) {
  TetFigure* t = all_game->figure;
  for (int i = 0; i < MY_FIGURES_SIZE; i++)    // i это y
    for (int j = 0; j < MY_FIGURES_SIZE; j++)  // j это x
      if (t->blocks[i * MY_FIGURES_SIZE + j] != 0) {
        // определяем координаты поля, соответствующие блоку и перенесем его
        // данные на поле
        int fx = t->x + j;
        int fy = t->y + i;
        all_game->tetg->field[fy][fx] = t->blocks[i * MY_FIGURES_SIZE + j];
      }
}

/**
 * @brief Проверка заполнения строк
 * @param i Указатель на строку
 * @param field Указатель на игровое поле.
 */
int lineFilledTet(int i, int** field) {
  int temp = 1;
  for (int j = 0; j < MY_FIELD_WIDTH; j++)
    if (field[i][j] == 0) temp = 0;
  return temp;
}

/**
 * @brief Cдвиг на одну строку вниз
 * @param i Указатель на строку
 * @param field Указатель на игровое поле.
 */
void dropLineTet(int i, int** field) {
  if (i == 0)
    for (int j = 0; j < MY_FIELD_WIDTH; j++) field[i][j] = 0;
  else
    // k - индекс текущей строки, k-1 - индекс верхней строки
    for (int k = i; k > 0; k--)
      for (int j = 0; j < MY_FIELD_WIDTH; j++)
        // переносим блоки верхней строки на строку текущую
        field[k][j] = field[k - 1][j];
}

/**
 * @brief Удаляет заполненные строки на игровом поле и рассчитывает очки.
 *
 * Эта функция проверяет каждую строку игрового поля на полное заполнение.
 * Если строка полностью заполнена, она удаляется, и игровое поле сдвигается
 * вниз. После этого функция подсчитывает количество удаленных строк и
 * возвращает соответствующее количество очков.
 *
 * @param tetg Указатель на GameInfo_t*.
 * @return Количество очков, полученных за удаление строк.
 */
int eraseLinesTet(GameInfo_t* tetg) {
  int** tf = tetg->field;
  int count = 0;
  for (int i = MY_FIELD_HIGHT - 1; i >= 0; i--)
    while (lineFilledTet(i, tf)) {
      dropLineTet(i, tf);  // удаляем dropNewFigure строку со сдвигом вниз
      count++;
    }

  switch (count) {
    case 0:
      count = 0;
      break;
    case 1:
      count = 100;
      break;
    case 2:
      count = 300;
      break;
    case 3:
      count = 700;
      break;
    default:
      count = 1500;
      break;
  }

  return count;
}

/**
 * @brief процедура выброса новой фигуры первый раз
 */
void dropNewFigure(AllGame* all_game) {
  all_game->figure->x = MY_FIELD_WIDTH / 2 - MY_FIGURES_SIZE / 2;
  all_game->figure->y = 0;

  // номер шаблона для фигуры определяется случайным образом
  int fnum = rand() % COUNT_OF_FIGUREST;

  // скопируем блоки шаблона в область фигуры
  for (int i = 0; i < MY_FIGURES_SIZE; i++)
    for (int j = 0; j < MY_FIGURES_SIZE; j++)
      all_game->figure->blocks[i * MY_FIGURES_SIZE + j] =
          all_game->tet_templates[fnum * MY_FIGURES_SIZE * MY_FIGURES_SIZE +
                                  i * MY_FIGURES_SIZE + j];

  // создание следующей фигуры
  fnum = rand() % COUNT_OF_FIGUREST;
  for (int i = 0; i < MY_FIGURES_SIZE; i++)
    for (int j = 0; j < MY_FIGURES_SIZE; j++)
      all_game->tetg->next[i][j] =
          all_game->tet_templates[fnum * MY_FIGURES_SIZE * MY_FIGURES_SIZE +
                                  i * MY_FIGURES_SIZE + j];
}

/**
 * @brief процедура выброса новой фигуры в цикле
 */
void dropNextNewFigure(AllGame* all_game) {
  all_game->figure->x = MY_FIELD_WIDTH / 2 - MY_FIGURES_SIZE / 2;
  all_game->figure->y = 0;
  //скопируем блоки next в область фигуры
  for (int i = 0; i < MY_FIGURES_SIZE; i++)
    for (int j = 0; j < MY_FIGURES_SIZE; j++)
      all_game->figure->blocks[i * MY_FIGURES_SIZE + j] =
          all_game->tetg->next[i][j];

  // создание следующей фигуры
  int fnum = rand() % COUNT_OF_FIGUREST;
  for (int i = 0; i < MY_FIGURES_SIZE; i++)
    for (int j = 0; j < MY_FIGURES_SIZE; j++)
      all_game->tetg->next[i][j] =
          all_game->tet_templates[fnum * MY_FIGURES_SIZE * MY_FIGURES_SIZE +
                                  i * MY_FIGURES_SIZE + j];
}

TetFigure* rotTetFigure(AllGame* all_game) {
  TetFigure* t = createTetFigure();
  TetFigure* told = all_game->figure;  // указатель на старую фигуру
  t->x = told->x;  // координаты фигуры при повороте не меняются
  t->y = told->y;
  for (int i = 0; i < MY_FIGURES_SIZE; i++)
    for (int j = 0; j < MY_FIGURES_SIZE; j++)
      t->blocks[i * MY_FIGURES_SIZE + j] =
          told->blocks[j * MY_FIGURES_SIZE +
                       (MY_FIGURES_SIZE - 1 -
                        i)];  // столбцы это строки старой фигуры, строки новой
                              // - это столбцы старой С КОНЦА!!
  return t;
}

void change_speed(GameInfo_t* tetg) {
  tetg->level = (tetg->score >= 6000) ? 10 : 1 + (tetg->score / 600);
  tetg->speed = VERYSLOWSPEED - (tetg->level * 2);
}

void input_high_score(int score) {
  FILE* file;
  file = fopen("brick_game/tetris/highScore.txt", "w");

  fprintf(file, "%d", score);
  fclose(file);
}

int read_high_score() {
  int res = 0;
  FILE* file;

  file = fopen("brick_game/tetris/highScore.txt", "r");
  fscanf(file, "%d", &res);
  fclose(file);

  return res;
}

void userInput(UserAction_t action, bool hold) {
  hold = true;
  action = hold;
  action -= 1;
}

#endif