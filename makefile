CC = gcc
CFLAGS = -Wall -W -std=c99
SRC = src/main.c src/gestion_lutins.c src/gestion_planning.c src/gestion_fichiers.c
OBJ = $(SRC:.c=.o)
EXEC = projet

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)


