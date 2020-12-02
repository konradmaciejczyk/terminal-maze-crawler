//Konrad Maciejczyk, 2020"
//clear && gcc main.c -o binaries/main -lncursesw && ./binaries/main
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>
#include <wchar.h>

void credits();
void set_layout();
void update_stats();
void game();
void how_to_play();
void hall_of_fame();

WINDOW *create_hud();
WINDOW *create_map();

void set_game_enviroment(){
   system("printf '\e[8;50;150t'"); //Setting user terminal to 150x50

   setlocale(LC_ALL, "");
   initscr(); //Initialize ncurses screen

   if (has_colors() == false){ //Check whether terminal can display colors
      printf("To play this game your terminal must support color display.");
      exit(1);
   }

   start_color(); //start colorful display
   init_pair(1, COLOR_WHITE, COLOR_BLACK); //Setting background color
}

void quit_game(){
   endwin();
   exit(0);
}

void display_banner(){
   int title_width = 94, title_height = 18;
   char title[] = "   _____                             _                          _                _           \n"
                  "  / ____|                           | |                        (_)              | |          \n"
                  " | (___   _   _  _ __    ___  _ __  | |_  ___  _ __  _ __ ___   _  _ __    __ _ | |          \n"
                  "  \\___ \\ | | | || '_ \\  / _ \\| '__| | __|/ _ \\| '__|| '_ ` _ \\ | || '_ \\  / _` || |          \n"
                  "  ____) || |_| || |_) ||  __/| |    | |_|  __/| |   | | | | | || || | | || (_| || |          \n"
                  " |_____/  \\__,_|| .__/  \\___||_|     \\__|\\___||_|   |_| |_| |_||_||_| |_| \\__,_||_|          \n"
                  "      _         | |                                                            _             \n"
                  "     | |        |_|                                                           | |            \n"
                  "   __| | _   _  _ __    __ _   ___   ___   _ __     ___  _ __  __ _ __      __| |  ___  _ __ \n"
                  "  / _` || | | || '_ \\  / _` | / _ \\ / _ \\ | '_ \\   / __|| '__|/ _` |\\ \\ /\\ / /| | / _ \\| '__|\n"
                  " | (_| || |_| || | | || (_| ||  __/| (_) || | | | | (__ | |  | (_| | \\ V  V / | ||  __/| |   \n"
                  "  \\__,_| \\__,_||_| |_| \\__, | \\___| \\___/ |_| |_|  \\___||_|   \\__,_|  \\_/\\_/  |_| \\___||_|   \n"
                  "                        __/ |                                                                \n"
                  "                       |___/                                                                 ";


   
   int row, col; getmaxyx(stdscr, row, col); //Getting current terminal height, width
   //Centering position of the banner
   int banner_pos_x = col/2 - title_width / 2;
   int banner_pos_y = row/2 - title_height;
   int banner_width = title_width + 3;
   int banner_height = title_height +3;

   WINDOW *banner = newwin(title_height, title_width, banner_pos_y, banner_pos_x); //Initialize window for title banner
   WINDOW *banner_foreground = newwin(banner_height, banner_width, banner_pos_y - 2, banner_pos_x - 2); //Initialize window for border under title
   refresh();  

   start_color();
   init_pair(2, COLOR_RED, COLOR_BLACK);//Setting 
   box(banner_foreground, 0, 0);
   wrefresh(banner_foreground); //Showing border around title banner   
   
   wattron(banner, COLOR_PAIR(2)); wattron(banner, A_BOLD);
   wprintw(banner, title); //Showing banner
   wrefresh(banner);	  
   wattroff(banner, COLOR_PAIR(2)); wattroff(banner, A_BOLD);	
}

void main_menu(){ 
   int row, col; getmaxyx(stdscr, row, col);

   int menu_width = 50, menu_height = 10, menu_pos_x = col/2 - 25, menu_pos_y = row - 22; //Creating window for opton menu_win
   WINDOW *menu_win = newwin(10, 50, menu_pos_y, menu_pos_x);
   refresh();
   wrefresh(menu_win);

   keypad(menu_win, true); //Allows to use arrows in order to pick up options from menu

   char MENU_OPTIONS[5][13] = {"New Game", "Hall of Fame", "How to play", "Credits", "Quit Game"};

   int highlight = 0, choice;

   while(1){ //main menu 
      for (int i=0; i<5; i++){
         if(i==highlight){
            wattron(menu_win, A_REVERSE);
         }
         
         mvwprintw(menu_win, i+1, 19, MENU_OPTIONS[i]);       
         wattroff(menu_win, A_REVERSE);       
      }
         choice = wgetch(menu_win);  

         switch (choice)
         {
         case KEY_UP:
            highlight--;
            if (highlight == -1)
               highlight = 0;
            break;
         case KEY_DOWN:
            highlight++;
            if (highlight == 4)
               highlight = 3;
            break;         
         default:
            break;
         }
         if(choice == 10){
            break;
         }
   }

   switch (highlight)
   {
   case 0:
      game();
      break;
   case 1:
      hall_of_fame();
      break;
   case 2:
      how_to_play();
      break;
   case 3:
      credits();
      break;
   case 4:
      quit_game();
   
   default:
      break;
   }    
}

void hall_of_fame(){
   printf("Hall of fame goes here!");
   quit_game();
}

void how_to_play(){
   printf("How to play goes here!");
   quit_game();
}
void credits(){
   int row, col;
   getmaxyx(stdscr, row, col); row /= 2; col /= 2;
   clear();
   mvprintw(row - 3, col - 6, "Created by:");
   mvprintw(row, col - 12, "Konrad Maciejczyk, 2020");
   mvprintw(row + 3, col - 14, "github.com/konradmaciejczyk");
   getch();
   clear();
   display_banner();
   main_menu();

}
void game(){
   clear(); refresh();
   WINDOW *hud, *map;
   
   
   hud = create_hud();
   map = create_map();

}

WINDOW *create_hud(){
   WINDOW *hud;
   init_pair(3, COLOR_YELLOW, COLOR_BLACK);
   int row, col; getmaxyx(stdscr, row, col);row = row / 2 - 22; col = col / 2 - 60;

   hud = newwin(44, 24, row, col);
   wborder(hud, ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ');

   mvwprintw(hud, 3, 5, "Player: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "Legend27");
   wattroff(hud, COLOR_PAIR(3));

   mvwprintw(hud, 12, 5, "health: ");
   wattron(hud, COLOR_PAIR(2)); wattron(hud, A_BOLD);
   wprintw(hud, "\u2665 \u2665 \u2665");
   wattroff(hud, COLOR_PAIR(2)); wattroff(hud, A_BOLD);

   mvwprintw(hud, 15, 5, "time: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "10:00");
   wattroff(hud, COLOR_PAIR(3));

   mvwprintw(hud, 18, 5, "score: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "0");
   wattroff(hud, COLOR_PAIR(3));

   mvwprintw(hud, 21, 5, "map: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "1");
   wattroff(hud, COLOR_PAIR(3));

   wrefresh(hud);
   return hud;
}

WINDOW *create_map(){
   WINDOW *map;

   int row, col; getmaxyx(stdscr, row, col); row = row / 2 - 23; col = col / 2 - 60;
   map = newwin(42, 83, row + 2, col + 28);

   int terrain[42][84];
   //zeroing out map matrix
   for(int i=1; i<41; i++){
      for(int j=2; j<82; j++){
         terrain[i][j] = 0;
      }
   }
   //vertical border lines
   for(int i=0; i<42; i++){
      terrain[i][0]  = 1; terrain[i][1]  = 1;
      terrain[i][81]  = 1; terrain[i][82]  = 1;
   }
   //horizontal border lines
   for(int i=0; i<84; i++){
      terrain[0][i]  = 1;
      terrain[41][i]  = 1;
   }

   //printing out obstacles
   for(int i=0; i<42; i++){
      for (int j=0; j<83; j++){
         if(terrain[i][j] == 1){
            mvwprintw(map, i, j, "\u2588");
         }
      }
   }
   
   wrefresh(map);

   return map;
}

int main() {
   set_game_enviroment();
   display_banner();
   main_menu();
   
   getch();		
	endwin();
	return 0;  
}