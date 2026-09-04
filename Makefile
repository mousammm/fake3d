BIN=renderer

all:
	gcc -ggdb -o $(BIN)  ./src/main.c -lSDL2 -lm

run:
	./$(BIN)

clean:
	rm ./$(BIN)
