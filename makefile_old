CC = gcc
OBJS = main.o mainAux.o parser.o solver.o game.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB)  -o $@ -lm
main.o: main.c mainAux.h parser.h solver.h game.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
mainAux.o: mainAux.c mainAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
parser.o: parser.c parser.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c solver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
game.o: game.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)