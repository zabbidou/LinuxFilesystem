EXEC=tema3
CFLAGS=-Wall -Wextra -g
CC=gcc
OBJ=main.o functions.o
HEADER=struct.h

build: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

run: $(EXEC)
	./$(EXEC)

force_build:
	$(CC) -o $(EXEC) *.c $(CFLAGS)

clean:
	rm *.o
