game: main.c map_elements.c
	rm -fr build/ && mkdir build
	clear && gcc *.c -o build/game -lncursesw
clean:
	rm build/game
debug:
	rm -fr build/ && mkdir build
	clear && gcc *.c -o build/game -lncursesw && gdb build/game
run:
	clear && ./build/game
