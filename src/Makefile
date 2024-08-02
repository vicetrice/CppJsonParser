# Variables
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17

# Source files
LIB_SRCS = Parser.cpp Rules.cpp JSONstruct.cpp JSONobject.cpp JSONarray.cpp JSON.cpp
LIB_OBJS = $(LIB_SRCS:.cpp=.o)
DEPS = Parser.hpp Rules.hpp Token.hpp JSONobject.hpp JSONarray.hpp JSONstruct.hpp JSON.hpp

# Default target
all: libparser.a

# Create the static library
libparser.a: $(LIB_OBJS)
	ar rcs $@ $(LIB_OBJS)

# Compile source files into object files
%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Clean up build artifacts
clean: clean-lib clean-obj

# Clean library file
clean-lib:
	$(RM) libparser.a

# Clean object files
clean-obj:
	$(RM) $(LIB_OBJS)

# Phony targets
.PHONY: all clean clean-lib clean-obj
