game: main.c map_elements.c
	clear && gcc *.c -o game -lncursesw
clean:
	rm game
debug:
	clear && gcc *.c -lncursesw && gdb game
run:
	clear && ./game
