
# 1. Compiler and Flags
CC       := gcc
CFLAGS   := -Wall -Wextra -g -O2
LDFLAGS  := 
LDLIBS   := 

# 2. Target Executable Name
TARGET   := my_program

# 3. Source and Object Files
# Automatically collects all .c files in the current directory
SRCS     := $(wildcard *.c)
OBJS     := $(SRCS:.c=.o)

# 4. Default Rule (Builds the application)
all: $(TARGET)

# 5. Linking the Executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# 6. Compiling Source Files to Object Files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 7. Clean Up Build Files
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
