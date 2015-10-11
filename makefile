# Compiler of choice
CC=g++

# Flags for the compiler
CFLAGS=-c -Wall -Wextra -pedantic -g -ggdb -Ofast -std=c++0x

# Flags for compilers that need invoke the linker (e.g. libraries)
LDFLAGS=

# Change filenames accordingly per project
SOURCES=$(wildcard *.cpp)
EXECUTABLE=p

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
