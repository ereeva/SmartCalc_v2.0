CC = g++ -std=c++17 -g
CPPFLAGS = -Wall -Werror -Wextra
GCOVFLAGS = --coverage
CLEAN	= rm -rf
TEXI2DVI = makeinfo --html

OS := $(shell uname)
ifeq ($(OS),Darwin)
	LEAKS = CK_FORK=no leaks --atExit --
	TESTFLAGS = -lgtest -lgtest_main -lm -lstdc++
	OPEN = open
	APP	= $(NAME).app
else
	LEAKS = valgrind --leak-check=full --track-origins=yes -s
	TESTFLAGS	= -lgtest -lgtest_main -lm -lstdc++ -lpthread
	OPEN = xdg-open
	APP	= $(NAME)
endif

NAME = SmartCalc

SRC_DIR	= ./model
SOURCE_FILES = *.cc

TEST_FILE	= $(NAME)_tests
TEST_FILE_COV	= $(TEST_FILE)_cov

TEST_DIR = ./tests
TEST_SOURCE_FILE = $(TEST_FILE).cc

SOURCE = $(SRC_DIR)/$(SOURCE_FILES) 
TEST = $(TEST_DIR)/$(TEST_SOURCE_FILE)

BUILD_DIR = build
DIST_NAME = $(NAME)

all: install

clean:
	$(CLEAN) $(TEST_FILE) $(TEST_FILE_FOR_LEAKS) $(TEST_FILE_COV) .clang-format
	$(CLEAN) *.o *.a *.gcno *.gcda *.info *.out *.gch *.dSYM report
	$(CLEAN) ../$(DIST_NAME).tar.gz ../$(DIST_NAME)
	$(CLEAN) ../documentation
	$(CLEAN) report*

$(TEST_FILE): model/calc.cc model/credit.cc $(TEST)
	$(CC) $(CPPFLAGS) model/calc.cc model/credit.cc $(TEST) -o $(TEST_FILE) $(TESTFLAGS)

test: $(TEST_FILE)
	./$(TEST_FILE)

rebuild: clean all

report/index.html: model/calc.cc model/credit.cc $(TEST)
	$(CC) $(CPPFLAGS) $(GCOVFLAGS) model/calc.cc model/credit.cc $(TEST) -o $(TEST_FILE_COV) $(TESTFLAGS)
	./$(TEST_FILE_COV)
	lcov -t "$(TEST_FILE_COV)" -o $(TEST_FILE_COV).info -c -d . --no-external --ignore-errors mismatch
	genhtml -o report $(TEST_FILE_COV).info

gcov_report: report/index.html
	open report/index.html

install: uninstall
	qmake CONFIG+=release -o $(BUILD_DIR)/Makefile view/$(NAME)V2.pro
	-cd $(BUILD_DIR); make
	-cd $(BUILD_DIR); make clean
	$(CLEAN) $(BUILD_DIR)/Makefile
	$(CLEAN) $(BUILD_DIR)/.qmake.stash
	$(CLEAN) $(BUILD_DIR)/qmake_qmake_qm_files.qrc
	$(CLEAN) $(BUILD_DIR)/.qm

uninstall: clean
	$(CLEAN) $(BUILD_DIR) documentation

dvi:
#	brew install doxygen    
	doxygen Doxyfile
	$(OPEN) ./documentation/html/index.html

dist: clean
	mkdir -p ./build
	tar -czvf ./build/SmartCalc.tar ./controller ./libs ./model ./view

check: style leaks

clang:
	clang-format -i --style=Google */*.cc */*.h

style:
	clang-format -n --style=Google */*.cc */*.h

cppcheck:
	cppcheck --std=c++17 --enable=all --language=c++ --suppress=missingIncludeSystem $(TEST) $(SOURCE)

leaks: $(TEST_FILE)
	$(LEAKS) ./$(TEST_FILE)

.PHONY: all clean test gcov_report clang style rebuild linter cppcheck leaks check