TARGET = typeordie
LIBS = -lm -lSDL -lSDL_mixer -lGL -lGLU -lglut -lstdc++
CC = gcc
CFLAGS = -std=c++11 -Werror -Wall -Wextra

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
