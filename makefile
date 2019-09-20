CC = gcc
OBJS = main.o main_Aux.o Stack.o Backtrack_Solver.o LP.o print.o sub_List.o List.o Interface.o UndoRedo.o Game.o Save_Load.o Commands.o LP_commands.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors 
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
Backtrack_Solver.o: Backtrack_Solver.h Backtrack_Solver.c Stack.h Game_board.h
	$(CC) $(COMP_FLAG) -c $*.c
LP.o: LP.h LP.c Game_board.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
print.o: print.c print.h Game_board.h
	$(CC) $(COMP_FLAG) -c $*.c
Save_Load.o: Save_Load.c Save_Load.h Game_board.h
	$(CC) $(COMP_FLAG) -c $*.c
sub_List.o: sub_List.c sub_List.h Game_board.h
	$(CC) $(COMP_FLAG) -c $*.c
List.o: List.c List.h sub_List.h
	$(CC) $(COMP_FLAG) -c $*.c
UndoRedo.o: UndoRedo.c UndoRedo.h List.h
	$(CC) $(COMP_FLAG) -c $*.c
Stack.o: Stack.c Stack.h
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c Game.h UndoRedo.h print.h Backtrack_Solver.h
	$(CC) $(COMP_FLAG) -c $*.c
LP_commands.o: LP_commands.c LP_commands.h Game_board.h UndoRedo.h List.h LP.h Save_Load.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Commands.o: Commands.c Commands.h Game.h Save_Load.h LP.h LP_commands.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
main_Aux.o: main_Aux.c main_Aux.h commands_modes.h Commands.h Interface.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
main.o: main.c Game.h LP_commands.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Interface.o: Interface.c Interface.h commands_modes.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
all: $(EXEC)