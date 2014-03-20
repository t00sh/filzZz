.PHONY: clean

VERSION = 1.0 beta
PACKAGE = filzZz

CC = gcc
CFLAGS = -O2 -Wall -Wextra -Wwrite-strings -Wstrict-prototypes -Wuninitialized
CFLAGS += -Wunreachable-code -g -fstack-protector-all
CFLAGS += -I include/
CFLAGS += -DVERSION="\"$(VERSION)\"" -DPACKAGE="\"$(PACKAGE)\""

SRC  = $(wildcard src/*.c)
OBJ  = $(SRC:%.c=%.o)

EXE = $(PACKAGE)

all: $(EXE)

$(EXE): $(OBJ)
	@echo " LINK $(EXE)" ;
	@$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIB);

%.o:%.c
	@echo " CC $@" ;
	@$(CC) $(CFLAGS) -c $< -o $@ ;

clean:
	rm $(EXE) $(OBJ)
