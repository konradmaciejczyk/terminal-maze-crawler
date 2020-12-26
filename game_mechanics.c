//Konrad Maciejczyk, 2020
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

extern int score; extern int game_time; extern void update_score(); extern WINDOW *hud;

void update_score(){
   wattron(hud, COLOR_PAIR(3));
   mvwprintw(hud, 18, 12, "%i", score);
   wattroff(hud, COLOR_PAIR(3));
   wrefresh(hud);
}

void draw_player(WINDOW *map, int prev_pos_x, int prev_pos_y, int pos_x, int pos_y){
    mvwprintw(map, prev_pos_x+1, prev_pos_y+2, " ");

    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    wattron(map, COLOR_PAIR(5)); wattron(map, A_BOLD);
    mvwprintw(map, pos_x+1, pos_y+2,"&");
    wattroff(map, COLOR_PAIR(5)); wattroff(map, A_BOLD);
    wrefresh(map);
}

int check_collision(int areas[40][80], int pos_x, int pos_y){
    //coin detection
    if(areas[pos_x][pos_y] == 4){
        score += 10;
        update_score();
        return 0;
    }else if(areas[pos_x][pos_y] == 5){
        score += 50;
        update_score();
        return 0;
    }

    //prevention against player leaving the map
    if (pos_x < 0 || pos_y < 0 || pos_x > 39 || pos_y > 79){
        return 1;
    }

    //obstacle detection
    if(areas[pos_x][pos_y]==1){
        return 1;
    }else{
        return 0;
    }
}

void player_movement(WINDOW *map, int areas[40][80], int *start_pos){
    game_time = (long double)game_time*CLOCKS_PER_SEC;
    keypad(map, true);
    int player_pos_x = 6; int player_pos_y = *start_pos;
    int previous_player_pos_x = player_pos_x; int previous_player_pos_y = player_pos_y;
    int key;
    long long time = 0;
    clock_t start, stop;
    nodelay(map, TRUE);    
    while(key = wgetch(map)){
        switch(key){
            case KEY_UP:{
                player_pos_x--;
                if (check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_x++;
                break;
            }
            case KEY_DOWN:{
                player_pos_x++;
                if (check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_x--;
                break;
            }
            case KEY_LEFT:{
                player_pos_y--;
                if (check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_y++;
                break;
            }
            case KEY_RIGHT:{
                player_pos_y++;
                if (check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_y--;
                break;
            }
            default:
                break;
        }
        areas[previous_player_pos_x][previous_player_pos_y] = 0; //player leaving area, leaves it with value 0
        areas[player_pos_x][player_pos_y] = 2; //updating players position
        draw_player(map, previous_player_pos_x, previous_player_pos_y, player_pos_x, player_pos_y);
        previous_player_pos_x = player_pos_x; previous_player_pos_y = player_pos_y;

        time += clock();
        mvwprintw(hud, 0, 0, "%Li, ", time/CLOCKS_PER_SEC); wrefresh(hud);      
    }

}