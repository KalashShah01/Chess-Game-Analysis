# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++11 -Wall

# Source file
SRC = main.cpp

# Executable name
EXEC = my_chess

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f $(EXEC)
