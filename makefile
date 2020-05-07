SHELL = /bin/bash

# Variables used for compiling sources
INC =$(wildcard inc/*.hpp)
SRC =$(wildcard src/*.cpp)
OBJ =$(SRC:src/%.cpp=obj/%.o)
CC = g++-9
OOPT = -Iinc -I../utils/inc -std=gnu++17 -g -c
LOPT = -L../utils/arc -lutils
TARGET = bin/alex

# link all object files to compiler
$(TARGET):$(OBJ)
	$(CC) $(OBJ) $(LOPT) -o $@

# compile every single source code document to object file
$(OBJ):obj/%.o:src/%.cpp $(INC)
	$(CC) $(OOPT) -o $@ $<

# initial project structure
init:
	if ! [ -e obj ]; then mkdir obj; fi
	if ! [ -e bin ]; then mkdir bin; fi

clean:
	rm -rf obj/*.o bin/*

install: $(TARGET)
	sudo cp bin/alex /usr/bin/alex

.PHONY: init clean install