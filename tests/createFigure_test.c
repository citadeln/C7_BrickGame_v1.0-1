#include "../brick_game/tetris/backend.h"
#include "test.h"

/* GCOV_createTetFigure_test_START */
START_TEST(createTetFigure_test) {
  TetFigure *t = createTetFigure();
  freeTetFigure(t);
}
END_TEST
/* GCOV_createTetFigure_test_STOP */

Suite *creatFigureTest(void) {
  Suite *s = suite_create("suite_Suite_creatFigureTest");
  TCase *tc = tcase_create("case_Suite_creatFigureTest");
  tcase_add_test(tc, createTetFigure_test);
  suite_add_tcase(s, tc);
  return s;
}