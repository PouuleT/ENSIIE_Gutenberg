# Paths
DSRC = src
DINC = inc
DOBJ = obj

#Compiler flags
CFLAGS = -W -Wall -g
LDFLAGS =

#Commands
CC = gcc $(CFLAGS)
SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)
AOUT = prog

all : $(AOUT)

prog : $(OBJS)
	$(CC) -o $@ $^ -lm
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean :
	@rm src/*.o
cleaner : clean
	@rm $(AOUT)
