#####################################################
# $Id: Makefile,v 1.33 2006/07/14 10:51:26 jochen Exp $
######################################################

CC = g++
#CC = /central/Software/gcc-4.0.2/bin/g++
#CC = /central/Software/gcc-4.1.0/bin/g++
#CC = icpc

#ERRFLAGS = -g -Wall -ansi -pedantic -Wextra 
ERRFLAGS = -g -Wall -ansi -pedantic -Wcast-align -Wshadow -Wconversion -DASSERTS #-DFET_COLSAMM
#ERRFLAGS = -g -Wall -ansi -pedantic --param large-function-growth=1000 --param inline-unit-growth=500 --param max-inline-insns-single=500
#ERRFLAGS = -g -Wall -ansi -pedantic -Winline
#ERRFLAGS = -g -Wall -ansi -pedantic -Winline --param large-function-growth=400000 --param inline-unit-growth=5000 --param max-inline-insns-single=10000
#ERRFLAGS = -g -w -ansi 
#OPTFLAGS =  -O3
#OPTFLAGS =  -O3 -Winline --param large-function-growth=300000 --param inline-unit-growth=100000 --param max-inline-insns-single=300000
#OPTFLAGS =  -g -Wall -Werror -ansi -pedantic

SOURCE = ../../Colsamm
COLFILES = ../../Colsamm/*

OBJ = 	main.o 

all: $(OBJ)
	$(CC) $(ERRFLAGS) $(OPTFLAGS) -o run $(OBJ) -lm

clean: ;rm *.o run

main.o: main.cc $(COLFILES)
	$(CC) -c $(ERRFLAGS) $(OPTFLAGS) main.cc

new: 
	@make clean
	@make 

time: 
	@time ./run

test: 
	@./run
