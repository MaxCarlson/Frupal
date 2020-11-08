CPPFLAGS = -g -Wall -Werror -std=c++17
TARGET = frupal
LIBS = -lncurses
SRCS = main.cpp map.cpp camera.cpp input.cpp item.cpp player.cpp ui.cpp display.cpp

$(TARGET): $(SRCS)
	g++ $(CPPFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)