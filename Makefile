game: main.c map_elements.c
	gcc main.c map_elements.c -o binaries/game -lncursesw
clean:
	rm binaries/game
run:
	clear && ./binaries/game
