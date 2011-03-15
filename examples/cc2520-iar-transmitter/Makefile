CC=msp430-gcc
CFLAGS=-Os -mmcu=msp430x5437 -Wall -ffunction-sections
LDFLAGS=-Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__ -mmcu=msp430x5437 -Wl,-Map=contiki-wismote.map
EXEC=cc2520
MAIN=main.c
OBJ=basic_rf.o hal_cc2520.o

all:$(EXEC).wismote

$(EXEC).wismote: $(OBJ)
	/opt/msp430-gcc-4.4.5/bin/msp430-ar rcf $(EXEC).a $^
	$(CC) -o main.o -c main.c $(CFLAGS)
	$(CC) -o $@ main.o $(EXEC).a $(LDFLAGS)
	
%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
.PHONY: clean mrproper 

clean:
	rm *.o
	
mrproper: clean
	rm $(EXEC)