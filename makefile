CC = g++ 
CFLAGS = -Wall -Werror
FILES = simulation.cpp load_config.cpp write_log.cpp

simulation: simulation.cpp
	$(CC) -o mysim $(FILES)

