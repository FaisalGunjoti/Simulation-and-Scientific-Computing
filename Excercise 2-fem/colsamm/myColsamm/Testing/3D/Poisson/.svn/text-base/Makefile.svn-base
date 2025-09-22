#####################################################
# $Id: Makefile,v 1.33 2006/07/14 10:51:26 jochen Exp $
######################################################

CC = $(CC_)

ERRFLAGS = $(ERRFLAGS_)
OPTFLAGS = $(OPTFLAGS_)
ADDFLAGS = $(ADDFLAGS_)

COLSAMM = -I../../../Source/
TESTSUITE = -I../../TestSuite/

#ERRFLAGS = -g -Wall -ansi -pedantic -Wcast-align -Wshadow -Wconversion -DFET_COLSAMM
#ERRFLAGS = -g -Wall -ansi -pedantic --param large-function-growth=1000 --param inline-unit-growth=500 --param max-inline-insns-single=500
#ERRFLAGS = -g -Wall -ansi -pedantic -Winline
#ERRFLAGS = -g -Wall -ansi -pedantic -Winline --param large-function-growth=400000 --param inline-unit-growth=5000 --param max-inline-insns-single=10000
#ERRFLAGS = -g -w -ansi 
#OPTFLAGS =  -O3
#OPTFLAGS =  -O3 -Winline --param large-function-growth=300000 --param inline-unit-growth=100000 --param max-inline-insns-single=300000
#OPTFLAGS =  -g -Wall -Werror -ansi -pedantic

all: 
	make "CC_ = $(CC)" "C_FLAGS = $(ERRFLAGS) $(OPTFLAGS) $(ADDFLAGS)" CuboidLaplacian
	make "CC_ = $(CC)" "C_FLAGS = $(ERRFLAGS) $(OPTFLAGS) $(ADDFLAGS)" TetrahedronLaplacian

clean: ;-rm CuboidLaplacian TetrahedronLaplacian 

CuboidLaplacian:	CuboidLaplacian.cc ../../TestSuite/testsuite.h
	$(CC) $(ERRFLAGS) $(OPTFLAGS) $(COLSAMM) $(TESTSUITE) -oCuboidLaplacian CuboidLaplacian.cc


TetrahedronLaplacian:	TetrahedronLaplacian.cc ../../TestSuite/testsuite.h
	$(CC) $(ERRFLAGS) $(OPTFLAGS) $(COLSAMM) $(TESTSUITE) -oTetrahedronLaplacian TetrahedronLaplacian.cc


new: 
	@make clean
	@make 

test:
	@echo 
	@echo "Running several benchmark file in order to prove correctness"
	@echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
	./CuboidLaplacian CuboidLaplacianData.dat
	@echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
	./TetrahedronLaplacian TetrahedronLaplacianData.dat
	@echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
	@echo 
	@echo "all tests are completed ..." 
	@echo 
