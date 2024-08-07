################## VARIABLES ###################

.SUFFIXES:

# Paths
HEAD = headers/
SRC = src/
COMP = compiled/
BIN = bin/


# vpath %.c $(SRC)
# vpath %.h $(HEAD)
# vpath %.o $(COMP)



# Compiler
DEBUGGING_FLAGS = -g -Wall
CFLAGS = -std=c99 -I$(HEAD)
OPTI_FLAGS = -O2
CC = gcc $(CFLAGS) $(DEBUGGING_FLAGS)   # or gcc $(CFLAGS) $(OPTI_FLAGS)

LFLAGS = -lm



# Valgrind
VFLAGS = --tool=memcheck --leak-check=full --track-origins=yes
VALGRIND = valgrind $(VFLAGS)



# Exectuables
PROGRAMS = $(BIN)main
TESTS = $(BIN)test_*

EXE = $(TESTS) $(PROGRAMS)


################## GENERATION ##################


# Generic -----------------------------------

$(COMP)%.o : $(SRC)%.c
	$(CC) -c $< -o $@


# Main --------------------------------------

main: $(COMP)main.o $(COMP)mapGenerator.o $(COMP)map.o $(COMP)chunk.o $(COMP)layer.o $(COMP)gradientGrid.o $(COMP)loadingBar.o $(COMP)interpreter.o
	$(CC) $^ -o $(BIN)$@ $(LFLAGS)

# Tests -------------------------------------

test_unicode: $(COMP)test_unicode.o
	$(CC) $^ -o $(BIN)$@

test_loadingBar: $(COMP)test_loadingBar.o $(COMP)loadingBar.o
	$(CC) $^ -o $(BIN)$@

test_gradientGrid: $(COMP)test_gradientGrid.o $(COMP)gradientGrid.o $(COMP)loadingBar.o $(COMP)interpreter.o
	$(CC) $^ -o $(BIN)$@ $(LFLAGS)

test_layer: $(COMP)test_layer.o $(COMP)layer.o $(COMP)gradientGrid.o $(COMP)loadingBar.o $(COMP)interpreter.o
	$(CC) $^ -o $(BIN)$@ $(LFLAGS)

test_chunk: $(COMP)test_chunk.o $(COMP)chunk.o $(COMP)layer.o $(COMP)gradientGrid.o $(COMP)loadingBar.o $(COMP)interpreter.o
	$(CC) $^ -o $(BIN)$@ $(LFLAGS)

test_map: $(COMP)test_map.o $(COMP)map.o $(COMP)chunk.o $(COMP)layer.o $(COMP)gradientGrid.o $(COMP)loadingBar.o $(COMP)interpreter.o
	$(CC) $^ -o $(BIN)$@ $(LFLAGS)

test_mapGenerator: $(COMP)test_mapGenerator.o $(COMP)mapGenerator.o $(COMP)map.o $(COMP)chunk.o $(COMP)layer.o $(COMP)gradientGrid.o $(COMP)loadingBar.o $(COMP)interpreter.o
	$(CC) $^ -o $(BIN)$@ $(LFLAGS)
	
test_interpreter: $(COMP)test_interpreter.o $(COMP)interpreter.o $(COMP)mapGenerator.o $(COMP)map.o $(COMP)chunk.o $(COMP)layer.o $(COMP)gradientGrid.o $(COMP)loadingBar.o
	$(CC) $^ -o $(BIN)$@ $(LFLAGS)

test_all : test_unicode test_loadingBar test_gradientGrid test_layer test_chunk test_map test_mapGenerator test_interpreter

# Valgrind ----------------------------------

# valgrind :
# 	$(VALGRIND) 

# Cleaning ----------------------------------

clean:
	rm -f $(COMP)*.o $(EXE)
