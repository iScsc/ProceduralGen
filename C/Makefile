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
TESTS = test_loadingBar test_gradientGrid test_layer test_chunk test_map

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

test_layer: test_layer.o layer.o gradientGrid.o loadingBar.o
	$(CC) test_layer.o layer.o gradientGrid.o loadingBar.o -o test_layer $(LFLAGS)

test_chunk: test_chunk.o chunk.o layer.o gradientGrid.o loadingBar.o
	$(CC) test_chunk.o chunk.o layer.o gradientGrid.o loadingBar.o -o test_chunk $(LFLAGS)

test_map: test_map.o map.o chunk.o layer.o gradientGrid.o loadingBar.o
	$(CC) test_map.o map.o chunk.o layer.o gradientGrid.o loadingBar.o -o test_map $(LFLAGS)

# Valgrind ----------------------------------

# valgrind :
# 	$(VALGRIND) 

# Cleaning ----------------------------------

clean:
	rm -f *.o $(EXE)
