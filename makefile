# Detecção Automática do Sistema Operacional onde o 'make' está rodando
ifeq ($(OS),Windows_NT)
    # Se estiver rodando nativamente no Windows
    TARGET := Roteador.exe
    RM     := del /Q
    CC     := gcc
else
    # Se estiver rodando nativamente no Linux / macOS
    TARGET := Roteador
    RM     := rm -f
    CC     := gcc
endif

# Se você estiver no Linux e quiser compilar PARA Windows,
# basta digitar: make env=windows
ifeq ($(env),windows)
    CC     := x86_64-w64-mingw32-gcc
    TARGET := my_program.exe
    RM     := rm -f
endif

# Flags de Compilação
CFLAGS   := -Wall -Wextra -g -O2
LDFLAGS  := 
LDLIBS   := 

# Arquivos Fonte e Objetos
SRCS     := $(wildcard *.c)
OBJS     := $(SRCS:.c=.o)

# Regras de Compilação
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza (usa o comando correto baseado no OS)
.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
