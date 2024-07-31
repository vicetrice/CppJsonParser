# Variables
CXX = g++ 
CXXFLAGS = -Wall  -Werror -std=c++17

# Source files
SRCS = Parser.cpp Rules.cpp JSONstruct.cpp main.cpp 
OBJS = $(SRCS:.cpp=.o)
DEPS = Parser.hpp Rules.hpp Token.hpp JSONobject.hpp JSONarray.hpp JSONstruct.hpp JSON.hpp

# Define platform-specific variables
ifeq ($(OS),Windows_NT)
    # Windows-specific settings
    RM = del /Q
    EXT = .exe
else
    # Unix-like settings (Linux/macOS)
    RM = rm -f
    EXT = 
endif

# Default target
all: parser debug

# Link object files into the final executable
parser: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Debug build
debug: $(OBJS)
	$(CXX) $(CXXFLAGS) -g -o $@ $(OBJS)

# Compile source files into object files
%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Clean up build artifacts
clean: clean-debug clean-parser clean-obj

# Clean debug executable
clean-debug:
	$(RM) debug$(EXT)

# Clean parser executable
clean-parser:
	$(RM) parser$(EXT)

# Clean object files
clean-obj:
	$(RM) $(OBJS)

# Phony targets
.PHONY: all clean debug clean-debug clean-parser clean-obj
