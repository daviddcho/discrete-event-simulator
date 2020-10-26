CC = g++ 
CFLAGS = -Wall -Werror
FILES = simulation.cpp 

simulation: simulation.cpp
	$(CC) -o mysim $(FILES)

