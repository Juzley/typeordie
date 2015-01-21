TARGET = bin/typeordie
CC = gcc
CFLAGS = -std=c++11 -Werror -Wall -Wextra -Wno-unused-parameter

ifeq ($(OS),Windows_NT)
	LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -lopengl32 -lvorbisfile -lvorbisenc -lvorbis -logg -lstdc++ 
else
	LIBS = -lGL -lSDL2 -lSDL2_mixer -lm -lstdc++
endif

.PHONY: default all clean

default: $(TARGET)

all: default

debug: CFLAGS += -g -O0 -D_DEBUG
debug: default


OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -r $(TARGET)
