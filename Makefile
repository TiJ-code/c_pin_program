CC=gcc
CFLAGS=-Iexternal/c_pin_ctrl_lib -Wall -Wextra -pedantic $(shell pkg-config --cflags pigpiod_if2)
LDFLAGS=-lpthread $(shell pkg-config --libs pigpiod_if2)

SRC=pin.c external/c_pin_ctrl_lib/pin_ctrl.c

.PHONY: all clean

all: build

build: 
	$(CC) $(SRC) $(CFLAGS) -o pin $(LDFLAGS)

clean:
	rm -f pin
