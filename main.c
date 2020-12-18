//Konrad Maciejczyk, 2020"
//clear && gcc main.c map_elements.c -o binaries/main -lncursesw && ./binaries/main
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

void credits(); void set_layout(); void update_stats(); void game(); void how_to_play(); void hall_of_fame();
WINDOW *create_hud(); WINDOW *create_map(); int create_terrain(); void draw_terrain(); void draw_end_point();

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

   int menu_width = 50, menu_height = 10, menu_pos_x = col/2 - 25, menu_pos_y = row - 22; //Creating window for option menu_win
   WINDOW *menu_win = newwin(10, 50, menu_pos_y, menu_pos_x);
   refresh();
   wrefresh(menu_win);

   keypad(menu_win, true); //Allows to use arrows in order to select options from menu

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
      //Getting player's name
      wclear(menu_win);
      int row, col; getmaxyx(menu_win, row, col); col = col / 2 - 8;
      mvwprintw(menu_win, 0, col, "Enter your name:");
      wmove(menu_win, 2, col + 4);
      char nazwa[9];
      wgetnstr(menu_win, nazwa, 8);   
      wrefresh(menu_win);

      game(nazwa);
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

void game(char nazwa[9]){
   clear(); refresh();
   WINDOW *hud, *map;   
   int start_end_point[2]; //player spawn point[0], level exit[1]
   hud = create_hud(nazwa);
   map = create_map();
   int areas[40][80];
   create_terrain(areas, start_end_point);
   draw_terrain(map, areas);
   draw_end_point(map, start_end_point[1]);
}

WINDOW *create_hud(char nazwa[9]){
   WINDOW *hud;
   init_pair(3, COLOR_YELLOW, COLOR_BLACK);
   int row, col; getmaxyx(stdscr, row, col);row = row / 2 - 22; col = col / 2 - 60;

   hud = newwin(44, 24, row, col);
   wborder(hud, ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ');
   //Player name
   mvwprintw(hud, 3, 5, "Player: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "%s", nazwa);
   wattroff(hud, COLOR_PAIR(3));
   //Players health
   mvwprintw(hud, 12, 5, "health: ");
   wattron(hud, COLOR_PAIR(2)); wattron(hud, A_BOLD);
   wprintw(hud, "\u2665 \u2665 \u2665");
   wattroff(hud, COLOR_PAIR(2)); wattroff(hud, A_BOLD);
   //Time left
   mvwprintw(hud, 15, 5, "time: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "10:00");
   wattroff(hud, COLOR_PAIR(3));
   //Score
   mvwprintw(hud, 18, 5, "score: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "0");
   wattroff(hud, COLOR_PAIR(3));
   //Map number
   mvwprintw(hud, 21, 5, "map: ");
   wattron(hud, COLOR_PAIR(3));
   wprintw(hud, "1");
   wattroff(hud, COLOR_PAIR(3));

   wrefresh(hud);
   return hud;
}

WINDOW *create_map(){
   WINDOW *map;
   //calculating place for centering the map 
   int row, col; getmaxyx(stdscr, row, col); row = row / 2 - 23; col = col / 2 - 60;
   map = newwin(42, 84, row + 2, col + 28);;
   for(int i=0; i<42; i++){//printing vertical borders
      mvwprintw(map, i, 0, "\u2588"); mvwprintw(map, i, 1, "\u2588");
      mvwprintw(map, i, 82, "\u2588"); mvwprintw(map, i, 83, "\u2588");
   }

   for(int i=2; i<82; i++){//printing horizontal borders
      mvwprintw(map, 0, i, "\u2588"); 
      mvwprintw(map, 41, i, "\u2588");
   }
   wrefresh(map);

   return map;
}

void make_block(int areas[40][80], int block_num, int x, int y){;
   switch (block_num){
      case 0:{ 
         extern int LR[10][20];
         for(int i=0; i<10; i++){
            for(int j=0; j<20; j++){
               areas[i + y][j + x] = LR[i][j];
            }
         }
         break;
      }
      case 1:{
         extern int LRD[10][20];
         for(int i=0; i<10; i++){
            for(int j=0; j<20; j++){
               areas[i + y][j + x] = LRD[i][j];
            }
         }
         break;
      }

      case 2:{;
         extern int LRT[10][20];
         for(int i=0; i<10; i++){
            for(int j=0; j<20; j++){
               areas[i + y][j + x] = LRT[i][j];
            }
         }
         break;
      }
      case 3:{
         extern int LRTD[10][20];
         for(int i=0; i<10; i++){
            for(int j=0; j<20; j++){
               areas[i + y][j + x] = LRTD[i][j];
            }
         }
         break;
      }
   }   
}
void make_random_block(int areas[40][80], int x, int y){
   make_block(areas, rand()%3, y, x); //creating random block from presets
   for(int i=x; i<x+10; i++){ //adding extra randomness on top of presets
      for(int j=y; j<y+19; j=j+2){
         if(rand()%25 < 2){
            areas[i][j] = 1; areas[i][j+1] = 1;
         }
      }
   }   
}

void draw_end_point(WINDOW *map, int end_point){
   printw("%i", end_point);
   init_pair(3, COLOR_YELLOW, COLOR_BLACK);   
   wattron(map, COLOR_PAIR(3));   
   mvwprintw(map, 35, end_point, "\u2588"); wprintw(map, "\u2588");
   wattroff(map, COLOR_PAIR(3));
   wrefresh(map);
}

int create_terrain(int areas[40][80], int start_end_point[2]){
   //zeroing out every of individual areas
   for(int i=0; i<40; i++){
      for(int j=0; j<80; j++)
         areas[i][j] = 0;
      }
   //creating first block
   srand(time(NULL));
   int prev_pos_x = rand()%4*20, prev_pos_y = 0;
   start_end_point[0] = prev_pos_x + 2;
   
   int block_num = rand()%4;
   make_block(areas, block_num, prev_pos_x, 0);

   bool stop_generate = false;
   int new_pos_x = prev_pos_x, new_pos_y = prev_pos_y;
   int direction = rand()%3;
   while(!stop_generate){
      //loop creating major, traversible path
      switch (direction){
         case 0: //move right
            if(new_pos_x < 60){
               new_pos_x = new_pos_x + 20;
               direction = rand()%3 == 1 ? 0:2;

               block_num = rand()%4;
               make_block(areas, block_num, new_pos_x, new_pos_y);               
            }
            else
               direction = rand()%3 == 0 ? 1:2;            
            break;
         case 1: //move left
            if(new_pos_x > 0){
               new_pos_x = new_pos_x - 20;
               direction = rand()%3 == 0 ? 1:2;

               block_num = rand()%4;
               make_block(areas, block_num, new_pos_x, new_pos_y);
            }
            else
               //direction = rand()%3 == 1 ? 0:2;            
               direction = 2;
            break;
         case 2: //move down
            if (new_pos_y < 30){
               if (block_num == 0 || block_num == 2)//0=LR, 2=LRT, 1=LRD, 3=LRTD
                  //making sure, that upper block has hole in the floor, so the player can transit between blocks
                  make_block(areas, rand()%2 == 0 ? 1:3, new_pos_x, new_pos_y);
               new_pos_y = new_pos_y + 10;
               direction = rand()%3;

               block_num = rand()%2 + 2;
               
               make_block(areas, block_num, new_pos_x, new_pos_y);
            }
            else
               stop_generate = !stop_generate;            
            break;         
      }
      //printw(", block_num=%i | ", block_num);
      
   }

   start_end_point[1] = new_pos_x+2;
   //loop for creating random maze
   for(int i=0; i < 40; i=i+10){
      for(int j=0; j < 80; j=j+20){
         if(areas[i][j] == 0){
            //printw("x:%i, y:%i | ", i, j);
            make_random_block(areas, i, j);
         }
      }
   }
   
}

void draw_terrain(WINDOW *map, int areas[40][80]){
   for(int i=0; i<40; i++){//rows
      for(int j=0; j<80; j++){//columns
         if(areas[i][j])
            mvwprintw(map, i+1, j+2, "\u2588");
      }
   }
   wrefresh(map);
}
int main() {
   set_game_enviroment();
   display_banner();
   main_menu();
   
   getch();		
	endwin();
	return 0;  
}