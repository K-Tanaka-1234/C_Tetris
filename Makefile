run:			build
				clear
				./tetris
build:			src/main.c src/display.c src/display.h src/keyboard.c src/keyboard.h src/tetris.c src/tetris.h
				gcc -o tetris -Wall src/main.c src/display.c src/keyboard.c src/tetris.c -lm
