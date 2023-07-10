
LIBS = -lm -lSDL2 #-lglib-2.0	# Libraries
CC = clang						# Compiler
CFLAGS = -g -Wall   			# -g for debugging, -Wall for warnings
TARGET = main					# Name of the executable

TEST_LIBS = -lcunit # Libraries needed for the test
TEST_TARGET = test

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


all: $(OBJECTS)
	echo "Linking..."
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)

.SILENT:
clean:
	echo "Cleaning..."
	-rm -f *.out
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -rf docs/html
	-rm -f $(TEST_TARGET)

rebuild: clean all

test: clean --test-pre  --test

--test-pre: 
	echo "Pre Build Test..."
	# adding following test flags to the CFLAGS
	$(eval CFLAGS += -DCTEST) 

# private target, cannot be called from outside, please call test
--test: $(OBJECTS)
	echo "Linking Test..."
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) $(TEST_LIBS) -o $(TEST_TARGET)

# Check if SDL2 is installed
check:
	echo "SDL Version: $(shell sdl2-config --version)"
	echo "Doxygen Version: $(shell doxygen --version)"

doc:
	@doxygen docs/doxy.dox
