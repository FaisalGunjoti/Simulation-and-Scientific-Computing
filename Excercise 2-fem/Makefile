#------------------------------------------------------------------------------
# Makefile for ex02 (Beam Waveguide, Assignment 2)
#
# Usage:
#   make            # builds ./waveguide
#   make clean      # removes all .o and the executable
#
#gcc/g++ flags: -Wall -std=c++20 -pedantic
# extra flags: to suppress warnings about deprecated 
#               enum conversions and parentheses from Colsamm headers
#------------------------------------------------------------------------------

CXX       := g++
CXXFLAGS  := -Wall -std=c++20 -pedantic -Wno-deprecated-enum-enum-conversion  -Wno-parentheses
INCLUDES  := -I./include
SRCDIR    := src
BUILDDIR  := build
TARGET    := waveguide

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

.PHONY: all clean directories

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm -rf $(BUILDDIR) $(TARGET)
