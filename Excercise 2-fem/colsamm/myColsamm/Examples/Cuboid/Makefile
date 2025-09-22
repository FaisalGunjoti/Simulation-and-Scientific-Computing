#####################################################
# $Id: Makefile,v 1.4 2006/02/20 07:33:53 jochen Exp $
######################################################
#CC = /central/Software/gcc-4.0.2/bin/g++
CC = g++
#CC = icpc

#ERRFLAGS = -w 
#GNUFLAGS = -pedantic
OPTFLAGS =  -O3 
#OPTFLAGS =  -g -Werror -Wall -ansi

SOURCE = ../../Source
COLFILES = $(SOURCE)/*

OBJ = 	main.o 

all: $(OBJ)
	$(CC) $(ERRFLAGS) $(GNUFLAGS) $(OPTFLAGS) -o run $(OBJ) -lm

c: ;rm *.o run

main.o: main.cc $(COLFILES)

	$(CC) -c $(ERRFLAGS) $(OPTFLAGS) main.cc
