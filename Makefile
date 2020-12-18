game: main.c map_elements.c
	gcc main.c map_elements.c -o binaries/game -lncursesw
clean:
	rm binaries/game
debug:
	gcc -g main.c map_elements.c -o binaries/game -lncursesw
run:
	clear && ./binaries/game
