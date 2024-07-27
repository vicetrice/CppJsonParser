# Variables
CXX = g++ 
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

# Source files
SRCS = Lexer.cpp Rules.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = Lexer.hpp Rules.hpp Token.hpp

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
all: lexer debug

# Link object files into the final executable
lexer: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Debug build
debug: $(OBJS)
	$(CXX) $(CXXFLAGS) -g -o $@ $(OBJS)

# Compile source files into object files
%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Clean up build artifacts
clean: clean-debug clean-lexer clean-obj

# Clean debug executable
clean-debug:
	$(RM) debug$(EXT)

# Clean lexer executable
clean-lexer:
	$(RM) lexer$(EXT)

# Clean object files
clean-obj:
	$(RM) $(OBJS)

# Phony targets
.PHONY: all clean debug clean-debug clean-lexer clean-obj
