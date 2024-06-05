#define IFDEBUG 0

#if IFDEBUG == 1

#include <stdio.h>
#include <unistd.h>

int main(void) {
  int i = 0;  // Счетчик секунд

  while (i < 7) {
    // Вывод строки (строка записывается в буфер)
    printf("\r%d", i);
    //Сброс буфера (строка отобразится в консоле)
    fflush(stdout);
    //Задержка на 1 секунду
    sleep(1);
    //Увеличение счетчика секунд на 1
    i++;
  }
  return 0;
}

// #include <stdio.h>
// #include <stdlib.h>

// #include "../brick_game/tetris/tetris.h"

// TetField* createTetField(int width, int height) {
//   TetField* tetf = (TetField*)malloc(sizeof(TetField));
//   tetf->width = width;
//   tetf->height = height;
//   tetf->blocks = (TetBlock*)malloc(sizeof(TetBlock) * width * height);

//   // изначально блоков нет, занулим параметр i каждого блока
//   for (int i = 0; i < width * height; i++) {
//     tetf->blocks[i].b = 0;
//     printf("%d ", tetf->blocks[i].b);
//   }

//   // printf("\nV2:\n");
//   // for (int i = 0; i < width * height; i++) {
//   //   tetf->blocks[i] = {1};
//   //   printf("%d ", tetf->blocks[i].b);
//   // }

//   return tetf;
// }

// TetGame* createTetGame(int field_width, int field_hight, int figures_size) {
//   TetGame* tetg = (TetGame*)malloc(sizeof(TetGame));

//   // инициализация переменных основной структуры
//   tetg->field = createTetField(field_width, field_hight);

//   return tetg;  // указатель на инициированную основную структуру TetGame
// }

// int main(int argc, char* argv[]) {
//   printf("Tetris!\n\n");

//   createTetGame(3, 5, 2);
//   return 0;
// }

#endif
