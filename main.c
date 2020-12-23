//Konrad Maciejczyk, 2020"
//clear && gcc *.c -o binaries/main -lncursesw && ./binaries/main
#include <stdlib.h>
#include <ncurses.h>

extern void set_game_enviroment(); extern void display_banner(); extern void main_menu();
extern WINDOW *create_hud(); extern WINDOW *create_map(); extern void create_terrain();
extern void draw_terrain(); extern void spawn_coins(); extern void player_movement();

void game(char nazwa[9]){
   clear(); refresh();

   int start_end_point[2]; //player spawn point[0], level exit[1]
   int areas[40][80]; //matrix representing level in cartesian coordinates

   WINDOW *hud, *map;   
   hud = create_hud(nazwa);
   map = create_map();
   
   create_terrain(areas, start_end_point);
   areas[6][start_end_point[0]] = 2; areas[35][start_end_point[1]] = 3; areas[35][start_end_point[1]+1] = 3;
   spawn_coins(map, areas); 
   draw_terrain(map, areas, start_end_point);

   player_movement(map, areas, &start_end_point[0]);   
}

int main() {
   set_game_enviroment();
   display_banner();
   main_menu();
   
   getch();		
	endwin();
	return 0;  
}