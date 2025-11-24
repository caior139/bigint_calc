# Compiler and flags
CC = gcc
CFLAGS = -O2 -Wall

# Arquivos fonte
SRC = calculadora.c numero.c numio.c numarit.c numutil.c

# Arquivos objeto correspondentes
OBJ = $(SRC:.c=.o)

# Nome do executável
EXE = calculadora

# Target padrão: compila tudo e gera executável
all: $(EXE)

# Linka todos os objetos em um executável
$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

# Regra genérica: compila .c para .o
%.o: %.c
	$(CC) -c $< $(CFLAGS)

# Limpar arquivos de compilação
clean:
	rm -f $(OBJ) $(EXE)
