#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdio.h>

#define TET_TICKS_START \
  20  // константа для количества тактов между падениями на 1 блок
#define MY_FIELD_WIDTH 10
#define MY_FIELD_HIGHT 20
#define MY_FIGURES_SIZE 4  // 4х4 размер квадрата в который помещается фигура
#define COUNT_OF_FIGUREST 7
#define TET_PLAYER_NOP -1  // бездействие
#define VERYSLOWSPEED 25  // коэффициент задеMY_FIELD_HIGHTржки 25 милисекунд
#define MAX_LENGTH_HIGHSCORE 20

typedef struct TetFigure {  // структура для падающей фигуры
  int x;
  int y;
  int blocks[MY_FIGURES_SIZE * MY_FIGURES_SIZE];  // содержит массив из блоков,
                                                  // соответствующих фигуре
} TetFigure;

// список возможных состояний игры
enum {
  TET_GAMEOVER = 0,  // игра завершена
  TET_PLAYING        // игра продолжается
};

typedef struct TetPlayer {
  int action;
} TetPlayer;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int** field;  // поле TetField
  int** next;   // следующая фигура в окне справа
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct TetGame {
  GameInfo_t* tetg;  ///< указатель на статичную структуру данных игры
  TetFigure* figure;  // падающая в данный момент фигура
  TetPlayer* player;   // задается пользователем
  int* tet_templates;  // как выглядят типовые фигуры
  int ticks;  // количество тактов между перемещениями
  int ticks_left;  // количество тактов до перемещения
  int playing;
} AllGame;

void game_loop();

void userInput(UserAction_t action, bool hold);
UserAction_t get_action();

GameInfo_t updateCurrentState();
GameInfo_t* get_field_info(AllGame* all_game);

int** createTetField();
void freeTetField(int** tetf);

AllGame* createAllGame();
void freeAllGame(AllGame* tetg);

GameInfo_t* createGameInfo_t();
void freeGameInfo_t(GameInfo_t* tetg);

TetFigure* createTetFigure();
void freeTetFigure(TetFigure* tf);

int** createNext();
void freeNext(int** next);

int* createTemplates();

void moveFigureDown(AllGame* all_game);
void moveFigureUp(AllGame* all_game);
void moveFigureRight(AllGame* all_game);
void moveFigureLeft(AllGame* all_game);

int collisionTet(AllGame* all_game);

// размещение фигуры после падения
void plantFigure(AllGame* all_game);

// проверка заполнения строк
int lineFilledTet(int i, int** tfl);

// сдвиг на одну строку вниз
void dropLineTet(int i, int** tfl);

// удаляет заполненные строки и подсчитывает их количество
int eraseLinesTet(GameInfo_t* tetg);

// процедура выброса новой фигуры
void dropNewFigure(AllGame* all_game);
void dropNextNewFigure(AllGame* all_game);

TetFigure* rotTetFigure(AllGame* all_game);

// расчет такта одного игрового цикла
void calculateTet(AllGame* all_game);

void change_speed(GameInfo_t* tetg);

void input_high_score(int score);
int read_high_score();

#endif