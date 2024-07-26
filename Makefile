# Variables
CXX = g++ # o usa clang++ si prefieres
CXXFLAGS = -Wall -Wextra -Werror -std=c++17


# Source files
SRCS = Lexer.cpp Rules.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = Lexer.hpp Rules.hpp Token.hpp

# Define platform-specific variables
ifeq ($(OS),Windows_NT)
    # Windows-specific settings
    RM = del /Q
    EXT =.exe
else
    # Unix-like settings (Linux/macOS)
    RM = rm -f
    EXT = 
endif

# Default target
all: lexer

# Link object files into the final executable
lexer: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files into object files
%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Clean up build artifacts
clean:
	$(RM) Lexer$(EXT) $(OBJS)

# Phony targets
.PHONY: all clean
