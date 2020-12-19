game: main.c map_elements.c
	clear && gcc *.c -o binaries/game -lncursesw && binaries/game
clean:
	rm binaries/game
debug:
	clear && gcc *.c -lncursesw && gdb binaries/game
run:
	clear && ./binaries/game
