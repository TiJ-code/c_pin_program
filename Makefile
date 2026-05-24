CC=gcc
CFLAGS=-Iexternal/c_pin_ctrl_lib -Wall -Wextra -pedantic
LDFLAGS=-lpigpiod_if2 -lpthread

SRC=pin.c external/c_pin_ctrl_lib/pin_ctrl.c

.PHONY: all clean

all: build

build: 
	$(CC) $(SRC) $(CFLAGS) -o pin $(LDFLAGS)

clean:
	rm -f pin
