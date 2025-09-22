#####################################################
# $Id: Makefile,v 1.1 2006/05/22 11:27:13 jochen Exp $
######################################################

CC = g++

ERRFLAGS = -g -Wall -ansi -pedantic -DFET_COLSAMM
OPTFLAGS =  -O3

SOURCE = ../../Source
COLFILES = $(SOURECE)/*

OBJ = 	main.o 

all: $(OBJ)
	$(CC) $(ERRFLAGS) $(OPTFLAGS) -o run $(OBJ) -lm

clean: ;rm *.o run

main.o: main.cc $(COLFILES)
	$(CC) -c $(ERRFLAGS) $(OPTFLAGS) -I$(SOURCE) main.cc

new: 
	@make clean
	@make 

time: 
	@time ./run

test: 
	@./run
