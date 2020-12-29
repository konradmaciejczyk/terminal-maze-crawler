//Konrad Maciejczyk, 2020"
//clear && gcc *.c -o binaries/main -lncursesw && ./binaries/main
#include <stdlib.h>
#include <ncurses.h>

extern void set_game_enviroment(); extern void display_banner(); extern void main_menu();
extern WINDOW *create_hud(); extern WINDOW *create_map(); extern void create_terrain();
extern void draw_terrain(); extern void spawn_coins(); extern bool player_movement();

int score = 0;
char players_name[9];
WINDOW *hud, *map;
int game_time = 10;
int health = 3; 
int map_num = 1;

void get_players_name(WINDOW *menu_win){
   wclear(menu_win);
   int row, col; getmaxyx(menu_win, row, col); col = col / 2 - 8;
   mvwprintw(menu_win, 0, col, "Enter your name:");
   wmove(menu_win, 2, col + 4);
   wgetnstr(menu_win, players_name, 8);   
   wrefresh(menu_win);
   curs_set(0);
}

void game(WINDOW *menu_win){

   int start_end_point[2]; //player spawn point[0], level exit[1]
   int areas[40][80]; //matrix representing level in cartesian coordinates
   //0 - free area, 1 - obstacle, 2 - player, 3 - level exit, 4 - ordinary coin, 5 - extraordinary coin, 6 - bomb
   get_players_name(menu_win);
   while(1){
      clear(); refresh(); 
      hud = create_hud();
      map = create_map();

      create_terrain(areas, start_end_point);
      areas[6][start_end_point[0]] = 2; areas[35][start_end_point[1]] = 3; areas[35][start_end_point[1]+1] = 3;
      spawn_coins(map, areas); 
      draw_terrain(map, areas, start_end_point);

      if(!player_movement(map, areas, &start_end_point[0])){
         printf("Game Over!"); exit(0);
      }
   }  
}

int main() {
   set_game_enviroment();
   display_banner();
   main_menu();
   
   getch();		
	endwin();
	return 0;  
}