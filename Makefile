# Variables
CXX = g++ # o usa clang++ si prefieres
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
TARGET = lexer

# Source files
SRCS = Lexer.cpp Rules.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

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
all: $(TARGET) 

# Link object files into the final executable
$(TARGET)$(EXT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	$(RM) $(TARGET)$(EXT) $(OBJS)

# Phony targets
.PHONY: all clean
