# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -pedantic -I./include
LDFLAGS = -lm

# Directories
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/lbm

# Find all .cpp files in the source directory
SRCS = $(wildcard $(SRCDIR)/*.cpp)
# Generate object file names from source file names
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Linking complete: $@"

# Compile source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled $<"

# Clean up build files
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILDDIR)

# Phony targets
.PHONY: all clean