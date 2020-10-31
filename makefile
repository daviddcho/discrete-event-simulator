CC = g++ 
CFLAGS = -Wall -Werror
FILES = simulation.cpp load_config.cpp handlers.cpp statistics.cpp time.cpp write_log.cpp create_event.cpp

mysim: simulation.cpp
	$(CC) $(FILES) $(CFLAGS)

