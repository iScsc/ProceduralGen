################## VARIABLES ###################

# Compiler
CFLAGS = -g -std=c99 -Wall
LFLAGS = -lm
CC = gcc $(CFLAGS)

# Valgrind
VFLAGS = --tool=memcheck --leak-check=full --track-origins=yes
VALGRIND = valgrind $(VFLAGS)

# Exectuables
PROGRAMS = main
TESTS = test_loadingBar test_gradientGrid test_map2d

EXE = $(TESTS) $(PROGRAMS)


################## GENERATION ##################


# Generic -----------------------------------

%.o : %.c
	$(CC) -c $< -o $@




# Tests -------------------------------------

test_loadingBar: test_loadingBar.o loadingBar.o
	$(CC) test_loadingBar.o loadingBar.o -o test_loadingBar

test_gradientGrid: test_gradientGrid.o gradientGrid.o loadingBar.o
	$(CC) test_gradientGrid.o gradientGrid.o loadingBar.o -o test_gradientGrid $(LFLAGS)

test_map2d: test_map2d.o map2d.o gradientGrid.o loadingBar.o
	$(CC) test_map2d.o map2d.o gradientGrid.o loadingBar.o -o test_map2d $(LFLAGS)

# Valgrind ----------------------------------

# valgrind :
# 	$(VALGRIND) 

# Cleaning ----------------------------------

clean:
	rm -f *.o $(EXE)
