CC = g++ 
CFLAGS = -Wall -Werror
FILES = simulation.cpp load_config.cpp handlers.cpp statistics.cpp

mysim: simulation.cpp
	$(CC) $(FILES) $(CFLAGS)

