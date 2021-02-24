//Konrad Maciejczyk, 2020
/*This file is responsible for menu display including, banner display, main menu, credits, options, quit game. 
Here I set terminal for optimal size (150, 50)*/
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>
#include <wchar.h>

void main_menu(); extern void game(); extern void quit_game();

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


   
   int row = 50, col=150; //Getting current terminal height, width
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


void hall_of_fame(){
   printf("Hall of fame goes here!");
   quit_game();
}

void how_to_play(){
   WINDOW *h_t_p = newwin(50, 150, 0, 0);
   wattron(h_t_p, A_BOLD); mvwprintw(h_t_p, 10, 72, "GOAL"); wattroff(h_t_p, A_BOLD);
   mvwprintw(h_t_p, 12, 27, "The point of this game is to make through the dungeon and get to the next level before time's up.");
   mvwprintw(h_t_p, 13, 28, "Collect coins, blast through maze, avoid explosions and finish the game with the highest score!");

   wattron(h_t_p, A_BOLD); mvwprintw(h_t_p, 17, 70, "CONTROLS"); wattroff(h_t_p, A_BOLD);
   mvwprintw(h_t_p, 19, 50, "ARROW UP                                Move up");
   mvwprintw(h_t_p, 20, 50, "ARROW DOWN                              Move down");
   mvwprintw(h_t_p, 21, 50, "ARROW LEFT                              Move left");
   mvwprintw(h_t_p, 22, 50, "ARROW RIGHT                             Move right");
   mvwprintw(h_t_p, 23, 50, "BACKSPACE                               Plant a bomb");
   mvwprintw(h_t_p, 24, 50, "HOME                                    Pause menu");

   init_pair(4, COLOR_BLUE, COLOR_BLACK); init_pair(1, COLOR_RED, COLOR_BLACK);
   init_pair(2, COLOR_YELLOW, COLOR_BLACK); init_pair(3, COLOR_MAGENTA, COLOR_BLACK);   
   wattron(h_t_p, A_BOLD); mvwprintw(h_t_p, 27, 72, "ICONS"); wattroff(h_t_p, A_BOLD);
   wattron(h_t_p, COLOR_PAIR(4)); mvwprintw(h_t_p, 29, 50, "&"); wattroff(h_t_p, COLOR_PAIR(4)); wprintw(h_t_p, "                                        You");
   wattron(h_t_p, COLOR_PAIR(1)); mvwprintw(h_t_p, 30, 50, "*"); wattroff(h_t_p, COLOR_PAIR(1)); wprintw(h_t_p, "                                        Bomb");
   wattron(h_t_p, COLOR_PAIR(2)); mvwprintw(h_t_p, 31, 50, "\u2022"); wattroff(h_t_p, COLOR_PAIR(2)); wprintw(h_t_p, "                                        Coin (x10)");
   wattron(h_t_p, COLOR_PAIR(3)); mvwprintw(h_t_p, 32, 50, "\u2022"); wattroff(h_t_p, COLOR_PAIR(3)); wprintw(h_t_p, "                                        Coin (x50)");
   wattron(h_t_p, COLOR_PAIR(2)); mvwprintw(h_t_p, 33, 50, "\u2588\u2588"); wattroff(h_t_p, COLOR_PAIR(2)); wprintw(h_t_p, "                                       Exit");

   wrefresh(h_t_p);
   getch();
   clear();
}

void credits(){
   int row=25, col=75;
   clear();
   mvprintw(row - 3, col - 6, "Created by:");
   mvprintw(row, col - 12, "Konrad Maciejczyk, 2020");
   mvprintw(row + 3, col - 14, "github.com/konradmaciejczyk");
   getch();
   clear();
   display_banner();
   main_menu();
}

void main_menu(){ 
   int row=50, col=150;

   int menu_width = 50, menu_height = 10, menu_pos_x = col/2 - 25, menu_pos_y = row - 22; //Creating window for option menu_win
   WINDOW *menu_win = newwin(10, 50, menu_pos_y, menu_pos_x);
   refresh();
   wrefresh(menu_win);

   keypad(menu_win, true); //Allows to use arrows in order to select options from menu
   char MENU_OPTIONS[4][13] = {"New Game", "How to play", "Credits", "Quit Game"};
   int highlight = 0, choice;

   while(1){ //main menu 
      for (int i=0; i<4; i++){
         if(i==highlight){
            wattron(menu_win, A_REVERSE);
         }
         
         mvwprintw(menu_win, i+1, 19, MENU_OPTIONS[i]);       
         wattroff(menu_win, A_REVERSE);       
      }
      choice = wgetch(menu_win);  

      switch (choice){
         case KEY_UP:
            highlight--;
            if (highlight == -1)
               highlight = 0;
            break;
         case KEY_DOWN:
            highlight++;
            if (highlight == 5)
               highlight = 4;
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
        game(menu_win);
        break;
    case 1:
        how_to_play();
        break;
    case 2:
        credits();
        break;
    case 3:
        quit_game();
    
    default:
        break;
   }    
}
