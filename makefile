#Makefile 5

PROGRAM_NAME = filesystems

IDIR=./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./src
LDIR=../lib

LIBS=-lm

_DEPS = $(PROGRAM_NAME).h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(PROGRAM_NAME).o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGRAM_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

run:
	@$(MAKE) && ./$(PROGRAM_NAME) $(ARGS)