CC=msp430-gcc
CFLAGS=-O0 -ggdb3 -mmcu=msp430x5437 -Wall
LDFLAGS=
EXEC=cc2520.wismote
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all:$(EXEC)

$(EXEC): $(OBJ)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
.PHONY: clean mrproper 

clean:
	rm *.o
	
mrproper: clean
	rm $(EXEC)