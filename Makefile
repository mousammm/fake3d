BIN=fake3d

all:
	gcc -ggdb -o $(BIN)  main.c -lSDL2 -lm

run:
	./$(BIN)

clean:
	rm ./$(BIN)
