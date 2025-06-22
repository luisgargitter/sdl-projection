# Default-Config
# Libraries
LIBS = -lm -lSDL2

# Compiler
CC = gcc

# Flags for compilations
# c			... compilation without linking
CFLAGS = -std=c11 -pedantic -Wall 			\
-D_DEFAULT_SOURCE -D_BSD_SOURCE				\
-D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L	\
-D_GNU_SOURCE -c

# Flags for linking
LFLAGS =

# Flags for debug compilation
# UNDEBUG	... undefines NDEBUG (enables asserts)
# g			... keep debug symbols
DCFLAGS = -UNDEBUG -DDEBUG -g
DLFLAGS = -g			

# Flags for production compilation
# O			... sets optimization-level
PC_FLAGS = -O2


BUILD_PATH = build/
SRC_PATH = src/

# Name of the executable
TARGET = main

DOXY_PATH = docs/
# Name of the doxy configuration
DOXY_CONF = .doxy.dox

# Format flag for indent
FORMAT = -linux


HEADERS = $(wildcard $(SRC_PATH)*.h)
SOURCES = $(wildcard $(SRC_PATH)*.c)

OBJECTS = $(patsubst $(SRC_PATH)%.c, $(BUILD_PATH)%.o, $(SOURCES))

$(BUILD_PATH)%.o: $(SRC_PATH)%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

all: --all-pre $(OBJECTS)
	echo "Linking..."
	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -o $(BUILD_PATH)$(TARGET)
	echo "Done."

--all-pre:
	mkdir -p $(BUILD_PATH)

prod: --prod-pre all

--prod-pre:
	echo "Adding production flags..."
	$(eval CFLAGS += $(PC_FLAGS))

debug: --debug-pre all

--debug-pre:
	echo "Adding debug flags..."
	$(eval CFLAGS += $(DCFLAGS))
	$(eval LFLAGS += $(DLFLAGS))

doxy: clean
	echo "Generating documentation..."
	doxygen $(DOXY_CONF) $(DOXY_PATH)
	echo "Done."

.SILENT:
clean:
	echo "Cleaning..."
	-rm -rf $(BUILD_PATH)
	-rm -rf $(DOXY_PATH)
	echo "Done."

# miscellaneous
format: 
	echo "Formatting header- and source-files..."
	clang-format -i $(SOURCES) $(HEADERS)
	echo "Done."
