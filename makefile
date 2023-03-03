
LIBS = -lm -lSDL2	# Libraries
CC = gcc			# Compiler
CFLAGS = -g -Wall	# -g for debugging, -Wall for warnings
TARGET = main		# Name of the executable

.PHONY: all clean	

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


all: $(OBJECTS)
	echo "Linking..."
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $(TARGET)

.SILENT:
clean:
	echo "Cleaning..."
	-rm -f *.out
	-rm -f *.o
	-rm -f $(TARGET)

rebuild: clean all

# Check if SDL2 is installed
check:
	echo "SDL Version: $(shell sdl2-config --version)"