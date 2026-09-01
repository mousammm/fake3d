BIN=renderer

all:
	gcc -o $(BIN)  main.c -lSDL2

run:
	./$(BIN)

clean:
	rm ./$(BIN)
