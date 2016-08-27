.PHONY: all clean

CC=gcc
BIN=./bin/
SOURCE=./src/

LIST=$(BIN)vpl_wexecution
LDLIBS+=`pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
CFLAGS+=-rdynamic -lstdc++ -std=c++11 -lm

all: pre-build main-build

clean: 
	rm -f $(LIST)

run: 
	$(LIST)

pre-build: clean

main-build: $(BIN)main

$(BIN)%: $(SOURCE)%.cpp
	$(CC) -o $(LIST) $(SOURCE)*.cpp $(LDLIBS) $(CFLAGS)