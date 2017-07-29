
ARCH=LINUX

CC=gcc
CFLAGS=-Wall -Wno-unused -O2 -funroll-loops -ffast-math -D$(ARCH)

EXE = retrocadeos
INC = -I../include
#LIB = -L/usr/local/lib -lalleg-4.0.3 -lalleg_unsharable
LIB = -L/usr/local/lib -lalleg

TARGETS = retrocadeos.o \
	 prefs.o \
	 cmdline.o \
	 menu.o \
	 bmp.o \
	 scr.o

all: $(EXE)
clean: 
	rm -f *.o 
	rm -f $(EXE) 
	rm -f *.log
	rm -f *~
	rm -f RUNGAME

$(EXE): $(TARGETS)
	$(CC) -o $(EXE) $(TARGETS) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
