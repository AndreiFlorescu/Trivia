all: trivia

trivia: trivia.c ./Util/init.c
	gcc -Wall trivia.c ./Util/init.c -o trivia -lcurses

run: trivia
	./trivia questions.txt savefile.txt

clean:
	rm trivia
