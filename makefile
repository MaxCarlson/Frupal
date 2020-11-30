CC = g++
TARGET = frupal
CPPFLAGS = -g -Wall -Werror -std=c++17
LIBS = -lncurses
SRCS = main.cpp map.cpp movement.cpp camera.cpp input.cpp item.cpp player.cpp ui.cpp display.cpp mapgenerator.cpp pathing.cpp
HEADERS = main.h map.h movement.h camera.h input.h item.h player.h ui.h display.h mapgenerator.h pathing.h
OBJECTS = main.o map.o movement.o camera.o input.o item.o player.o ui.o display.o mapgenerator.o pathing.o

%.o: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)

$(TARGET): $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *.o
	rm -f $(TARGET)
