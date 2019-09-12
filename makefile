CC = gcc
OBJS = main.o Stack.o Backtrack_Solver.o ILP_solver.o print.o sub_List.o List.o UndoRedo.o Game.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall 
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
Backtrack_Solver.o: Backtrack_Solver.h Backtrack_Solver.c Stack.h Game_Board.h
	$(CC) $(COMP_FLAG) -c $*.c
ILP_solver.o: ILP_solver.h ILP_solver.c Game_Board.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
print.o: print.c print.h Game_Board.h
	$(CC) $(COMP_FLAG) -c $*.c
sub_List.o: sub_List.c sub_List.h Game_Board.h
	$(CC) $(COMP_FLAG) -c $*.c
List.o: List.c List.h sub_List.h
	$(CC) $(COMP_FLAG) -c $*.c
UndoRedo.o: UndoRedo.c UndoRedo.h List.h
	$(CC) $(COMP_FLAG) -c $*.c
Stack.o: Stack.c Stack.h
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c Game.h UndoRedo.h print.h Backtrack_Solver.h
	$(CC) $(COMP_FLAG) -c $*.c
main.o: main.c Game.h ILP_solver.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
