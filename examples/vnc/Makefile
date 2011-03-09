CONTIKI_PROJECT = vnc-example
all: $(CONTIKI_PROJECT)

CONTIKI_SOURCEFILES += ctk-vncfont.c vnc-server.c libconio.c vnc-out.c ctk-vncserver.c ctk.c

APPS = vnc

CLEAN += $(CONTIKI_PROJECT).$(TARGET) $(CONTIKI_PROJECT).hex

CONTIKI = ../..
include $(CONTIKI)/Makefile.include
