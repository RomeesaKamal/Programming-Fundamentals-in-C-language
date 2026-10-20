CC      = gcc
TARGET  = Main
SRCS    = Main.c Menu.c
CFLAGS  = $(shell pkg-config --cflags gtk+-3.0)
LIBS    = $(shell pkg-config --libs   gtk+-3.0)

$(TARGET): $(SRCS) Menu.h
	$(CC) $(SRCS) $(CFLAGS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
