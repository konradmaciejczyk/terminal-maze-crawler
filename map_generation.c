//Konrad Maciejczyk, 2020
/*This file contains all map generation procedures including: creating path between player spawn point end level, random obstacle creation,
coin spawning, player and level exit spawning.
*/
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

WINDOW *create_hud(char nazwa[9]){
   int row = 3, col =15;
   WINDOW *hud = newwin(44, 24, row, col);
   init_pair(3, COLOR_YELLOW, COLOR_BLACK);

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
   int row = 2, col = 15;
   map = newwin(42, 84, row + 2, col + 28);

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

void make_block(int areas[40][80], int block_num, int x, int y){
   int *block;
   switch (block_num){
      case 0:{ 
         extern int LR[10][20];
         block = &LR[0][0];
         break;
      }
      case 1:{
         extern int LRD[10][20];
         block = &LRD[0][0];
         break;
      }

      case 2:{
         extern int LRT[10][20];
         block = &LRT[0][0];
         break;
      }

      case 3:{
         extern int LRTD[10][20];
         block = &LRTD[0][0];
         break;
      }
   }
   
   int ptr_i = 0;
   for(int i=0; i<10; i++){
      for(int j=0; j<20; j++){
         areas[i + y][j + x] = *(block + ptr_i);
         ptr_i++;
         }
   }
}

void make_random_block(int areas[40][80], int x, int y){
   make_block(areas, rand()%3, y, x); //creating random block from presets
   for(int i=x; i<x+10; i++){ //adding extra randomness on top of presets
      for(int j=y; j<y+19; j=j+2){
         if(rand()%20 < 2){
            areas[i][j] = 1; areas[i][j+1] = 1;
         }
      }
   }   
}

void create_terrain(int areas[40][80], int start_end_point[2]){
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
               direction = rand()%3 == 1 ? 0:2;
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
   }   

   start_end_point[1] = new_pos_x+16;

   //loop for creating random maze
   for(int i=0; i < 40; i=i+10){
      for(int j=0; j < 80; j=j+20){
         if(areas[i][j] == 0){
            make_random_block(areas, i, j);
         }
      }
   }   
}

void draw_terrain(WINDOW *map, int areas[40][80], int start_end_point[2]){
   init_pair(3, COLOR_GREEN, COLOR_BLACK);
   init_color(COLOR_MAGENTA, 790, 50, 750); init_pair(1, COLOR_MAGENTA, COLOR_BLACK); 
   for(int i=0; i<40; i++){//rows
      for(int j=0; j<80; j++){//columns
         if(areas[i][j] == 1)//draw obstacle
            mvwprintw(map, i+1, j+2, "\u2588");
         else if(areas[i][j] == 4){//draw ordinary coin
            wattron(map, COLOR_PAIR(3));
            mvwprintw(map, i+1, j+2, "\u2022");
            wattroff(map, COLOR_PAIR(3));
         }
         else if(areas[i][j] == 5){//draw extraordinary coin
            wattron(map, COLOR_PAIR(1));
            mvwprintw(map, i+1, j+2, "\u2022");
            wattroff(map, COLOR_PAIR(1));
         }
      }
   }
   //player
   init_pair(5, COLOR_BLUE, COLOR_BLACK); //\u263A
   wattron(map, COLOR_PAIR(5)); wattron(map, A_BOLD); //wattron(map, A_STANDOUT);
   mvwprintw(map, 7, start_end_point[0]+2, "&");
   wattroff(map, COLOR_PAIR(5)); wattroff(map, A_BOLD); //wattroff(map, A_STANDOUT);
   wrefresh(map);
   //level exit
   init_pair(3, COLOR_YELLOW, COLOR_BLACK);   
   wattron(map, COLOR_PAIR(3));   
   mvwprintw(map, 36, start_end_point[1]+2, "\u2588"); wprintw(map, "\u2588");
   wattroff(map, COLOR_PAIR(3));
   wrefresh(map);
}

void spawn_coins(WINDOW *map, int areas[40][80]){   
   for(int i=0; i<40; i++){
      for(int j=0; j<80; j++){
         if(areas[i][j] == 0){
            if(rand()%40 < 2)
               areas[i][j] = 4;
            else if(rand()%250 < 2)
               areas[i][j] = 5;
         }
      }
   }   
}