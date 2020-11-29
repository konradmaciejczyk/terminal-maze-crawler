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
WINDOW *create_hud();

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

   char MENU_OPTIONS[4][13] = {"New Game", "Hall of Fame", "Credits", "Quit Game"};

   int highlight = 0, choice;

   while(1){
      for (int i=0; i<4; i++){
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
         //update_stats(my_win);
      break;
   case 1:
      
      break;
   case 2:
      credits();
      break;
   case 3:
      quit_game();
      break;
   
   default:
      break;
   }    
}

void credits(){
   clear();
   mvprintw(22, 71, "Made by:");
   mvprintw(23, 63, "Konrad Maciejczyk, 2020");
   mvprintw(27, 61, "github.com/konradmaciejczyk");
   getch();
   clear();
   display_banner();
   main_menu();

}
void game(){
   clear(); refresh();
   WINDOW *hud;
   
   
   hud = create_hud();

}

WINDOW *create_hud(){
   WINDOW *hud;
    
   hud = newwin(42, 24, 1, 2);
   wborder(hud, ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ');
   mvwprintw(hud, 3, 5, "Player: %s", "Konrad");
   mvwprintw(hud, 12, 5, "health: "); wprintw(hud, "\u2665 \u2665 \u2665");
   mvwprintw(hud, 15, 5, "time: 15:00");
   mvwprintw(hud, 18, 5, "score: 10");
   mvwprintw(hud, 21, 5, "map: 12");

   wrefresh(hud);
   return hud;
}

void update_stats(WINDOW *hud){
   mvwprintw(hud, 3, 4, "time: 15:00");
}

int main() {
   set_game_enviroment();
   display_banner();
   main_menu();
   
   getch();		
	endwin();
	return 0;  
}