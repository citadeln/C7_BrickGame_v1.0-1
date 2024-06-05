TARGET = tetris
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g
EXTRA_FLAG = -lncurses -lrt -D_POSIX_C_SOURCE=199309L
GCOVFLAGS = -fprofile-arcs -ftest-coverage

PREF_SRC = ./brick_game/tetris
PREF_OBJ = ./obj/
FRONTEND = gui/cli/

DEBUG_DIR = debug
DEBUG_NAME = debug.out

INSTALL_DIR = GAME

SRC_TEST = tests/test.c
LIBS_TEST = -lcheck -lsubunit -lm -lgcov

SRC = $(shell find $(PREF_SRC) $(FRONTEND) -name '*.c')
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(PREF_OBJ)%.o: $(PREF_SRC)%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@

install: $(OBJ)
	mkdir $(INSTALL_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(INSTALL_DIR)/$(TARGET) $(EXTRA_FLAG)

uninstall: 
	rm -rf $(INSTALL_DIR)

dvi:
	open ./doxygen/html/index.html

dist:
	tar -cvf BrickGame_v1.0-1.tar .

style:
	cp ../materials/linters/.clang-format .
	find . -name '*.[ch]' | xargs clang-format -n
	find . -name '*.[ch]' | xargs clang-format -i
	rm .clang-format

test: clean 
	@rm -rf build
	@mkdir -p build
	@$(CC) $(CFLAGS) brick_game/tetris/backend.c tests/*.c -L. -L/usr/local/lib -L/usr/include $(GCOVFLAGS) $(LIBS_TEST) -o build/test_tetris $(EXTRA_FLAG)
	@build/test_tetris

gcov_report: test
#	~/.local/bin/gcovr
	gcovr -r . --html --html-details -o build/testTetris.html
	@xdg-open build/testTetris.html


clean:
	rm -rf $(TARGET) $(PREF_OBJ) $(DEBUG_DIR)/$(DEBUG_NAME) $(INSTALL_DIR) BrickGame_v1.0-1.tar build $(ТESTS_GCOV_NAME) *.gcda *.gcno testTetris.css testTetris.html

debug: clean $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(DEBUG_DIR)/$(DEBUG_NAME)

cpp:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem brick_game/tetris/*.c gui/cli/*.c

val: install
	valgrind --tool=memcheck --leak-check=yes ./$(INSTALL_DIR)/$(TARGET)