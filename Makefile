BIN=renderer

all:
	gcc -o $(BIN)  ./src/main.c -lSDL3 -lm

run:
	./$(BIN)

clean:
	rm ./$(BIN)
