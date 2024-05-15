################## VARIABLES ###################

# Compiler
DEBUGGING_FLAGS = -g -Wall
CFLAGS = -std=c99
OPTI_FLAGS = -O2
CC = gcc $(CFLAGS) $(DEBUGGING_FLAGS)   # or gcc $(CFLAGS) $(OPTI_FLAGS)

LFLAGS = -lm

# Valgrind
VFLAGS = --tool=memcheck --leak-check=full --track-origins=yes
VALGRIND = valgrind $(VFLAGS)

# Exectuables
PROGRAMS = main
TESTS = test_unicode test_loadingBar test_gradientGrid test_layer test_chunk test_map test_mapGenerator

EXE = $(TESTS) $(PROGRAMS)


################## GENERATION ##################


# Generic -----------------------------------

%.o : %.c
	$(CC) -c $< -o $@




# Tests -------------------------------------

test_unicode: test_unicode.o
	$(CC) test_unicode.c -o test_unicode

test_loadingBar: test_loadingBar.o loadingBar.o
	$(CC) test_loadingBar.o loadingBar.o -o test_loadingBar

test_gradientGrid: test_gradientGrid.o gradientGrid.o loadingBar.o
	$(CC) test_gradientGrid.o gradientGrid.o loadingBar.o -o test_gradientGrid $(LFLAGS)

test_layer: test_layer.o layer.o gradientGrid.o loadingBar.o
	$(CC) test_layer.o layer.o gradientGrid.o loadingBar.o -o test_layer $(LFLAGS)

test_chunk: test_chunk.o chunk.o layer.o gradientGrid.o loadingBar.o
	$(CC) test_chunk.o chunk.o layer.o gradientGrid.o loadingBar.o -o test_chunk $(LFLAGS)

test_map: test_map.o map.o chunk.o layer.o gradientGrid.o loadingBar.o
	$(CC) test_map.o map.o chunk.o layer.o gradientGrid.o loadingBar.o -o test_map $(LFLAGS)

test_mapGenerator: test_mapGenerator.o mapGenerator.o map.o chunk.o layer.o gradientGrid.o loadingBar.o
	$(CC) test_mapGenerator.o mapGenerator.o map.o chunk.o layer.o gradientGrid.o loadingBar.o -o test_mapGenerator $(LFLAGS)

# Valgrind ----------------------------------

# valgrind :
# 	$(VALGRIND) 

# Cleaning ----------------------------------

clean:
	rm -f *.o $(EXE)
