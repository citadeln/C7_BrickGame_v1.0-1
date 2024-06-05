#include "frontend.h"

void printGameInfo_t(AllGame* all_game) {
  TetFigure* t = all_game->figure;

  if (all_game->tetg->pause) {
    make_pause();
    all_game->tetg->pause = 0;
  }

  for (int i = 0; i < MY_FIELD_HIGHT; i++)
    for (int j = 0; j < MY_FIELD_WIDTH; j++) {
      int sym = 1;                           // цветовая схема
      if (all_game->tetg->field[i][j] != 0)  // заменить ли? на != 0
        sym = 2;                             // цветовая схема
      else {
        // проверим, располагается ли блок фигуры по данным координатам
        int x = j - t->x;
        int y = i - t->y;
        if (x >= 0 && x < MY_FIGURES_SIZE && y >= 0 && y < MY_FIGURES_SIZE)
          if (t->blocks[y * MY_FIGURES_SIZE + x]) sym = 2;
      }
      attron(COLOR_PAIR(sym));
      mvaddch(i, j, ' ');  // выводим пробел в точке с координатами i и j
      attroff(COLOR_PAIR(sym));  // вернем старую цветовую схему
      // printf("%d", sym);
    }
}

// Для выделения секунд до столкновения с нижней точкой поля
void sleep_milli(int milliseconds) {
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = milliseconds * 1000 * 1000;
  nanosleep(&ts, NULL);
}

void make_pause() {
  while (getch() != ' ') {
    mvprintw(MY_FIELD_HIGHT + 1, MY_FIELD_WIDTH / 2 - 3,
             "Press SPACE to pause / continue...");
  }
}

void make_start() {
  while (getch() != '\n') {
    mvprintw(MY_FIELD_HIGHT + 1, MY_FIELD_WIDTH / 2 - 3,
             "Press ENTER to start");
    clrtoeol();
  }
  mvprintw(MY_FIELD_HIGHT + 1, MY_FIELD_WIDTH / 2 - 3,
           "Press SPACE to pause / continue...");
}

void start_ncurses() {
  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_CYAN);  // первая цветовая пара для текста и
                                          // соответствующего ему цветового фона
  init_pair(2, COLOR_GREEN, COLOR_BLUE);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  cbreak();  // строковый буфер, принуждает к немедленному выводу символа
             // нажатой клавиши
  noecho();  // отключает вывод символа нажатой клавиши на экран
  nodelay(stdscr, TRUE);  // отключает задержку при вызове функции getch()
  scrollok(stdscr, TRUE);  // отключает возможность скролинга
  keypad(stdscr, TRUE);  // чтоб работали клавиши
}

void print_score_level_speed(GameInfo_t* tetg) {
  //вывести количество очков
  attron(COLOR_PAIR(1));
  mvprintw(0, MY_FIELD_WIDTH + 5, "Score: %d", tetg->score);

  mvprintw(2, MY_FIELD_WIDTH + 5, "High score: %d", read_high_score());

  mvprintw(4, MY_FIELD_WIDTH + 5, "Level: %d", tetg->level);

  mvprintw(6, MY_FIELD_WIDTH + 5, "Speed: %d", 600 / tetg->speed);

  // переместим курсор за пределы поля
  move(MY_FIELD_HIGHT + 1, 0);
}

void print_next_figure(GameInfo_t* tetg) {
  mvprintw(9, MY_FIELD_WIDTH + 5, "Next figure:");

  // переместим курсор
  move(11, MY_FIELD_WIDTH + 5);

  // вывод следующей фигуры
  attroff(COLOR_PAIR(2));
  for (int i = 0; i < MY_FIGURES_SIZE; ++i) {
    for (int j = 0; j < MY_FIGURES_SIZE; ++j) {
      char symbol = (tetg->next[i][j]) ? 'o' : ' ';
      mvaddch(i + 11, MY_FIELD_WIDTH + 5 + j * 2, symbol);
    }
  }
}
