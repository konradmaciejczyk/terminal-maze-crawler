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

void update_time(int timer){
    wattron(hud, COLOR_PAIR(3));
    if(timer%60 < 10)//making it look nicer by puting 0 in front of seconds
        mvwprintw(hud, 15, 11, "0%i:0%i", timer/60, timer%60);
    else
        mvwprintw(hud, 15, 11, "0%i:%i", timer/60, timer%60);
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
    //prevention against player leaving the map
    if (pos_x < 0 || pos_y < 0 || pos_x > 39 || pos_y > 79){
        return 1;
    }
    //prevention against stepping on a bomb
    if(areas[pos_x][pos_y] == 6)
        return 1;
    //obstacle detection
    if(areas[pos_x][pos_y]==1){
        return 1;
    }else{
        return 0;
    }
}

bool plant_a_bomb(WINDOW *map, int areas[40][80], int direction, int pos_x, int pos_y){
    init_pair(2, COLOR_RED, COLOR_BLACK);
    wattron(map, COLOR_PAIR(2));
    bool aux;
    switch (direction){
    case 0: //up
        if(!check_collision(areas, pos_x - 1, pos_y)){
            mvwprintw(map, pos_x, pos_y + 2, "\u2605");
            areas[pos_x-1][pos_y] = 6; aux = true;
        } break;
    case 1: //down
        if(!check_collision(areas, pos_x + 1, pos_y)){
            mvwprintw(map, pos_x + 2, pos_y + 2, "\u2605");
            areas[pos_x+1][pos_y] = 6; aux = true;
        } break;
    case 2: //left
        if(!check_collision(areas, pos_x, pos_y - 1)){
            mvwprintw(map, pos_x+1, pos_y + 1, "\u2605");
            areas[pos_x][pos_y-1] = 6; aux = true;
        } break;
    case 3: //right
        if(!check_collision(areas, pos_x, pos_y + 1)){
            mvwprintw(map, pos_x+1, pos_y + 3, "\u2605");
            areas[pos_x][pos_y+1] = 6; aux = true;
        } break;
    }
    wrefresh(map);
    wattroff(map, COLOR_PAIR(2));
    return aux;   
}

void detect_a_coint(int areas[40][80], int pos_x, int pos_y){
    if (areas[pos_x][pos_y] == 4){
        score += 10;
        update_score();
    }else if(areas[pos_x][pos_y] == 5){
        score += 50;
        update_score();
    }
}

bool bomb_countdown(long long planted_at, long long now){
    if(now - planted_at >= 2){
        printw("!!!"); refresh();
        return false;
    }
    return true;
}

void player_movement(WINDOW *map, int areas[40][80], int *start_pos){
    int last_direction = 0; //determinates a direction player currently is pointing     
    int player_pos_x = 6; int player_pos_y = *start_pos;
    int previous_player_pos_x = player_pos_x; int previous_player_pos_y = player_pos_y;
    long long current = (long long)time(NULL);
    long long now = current;
    bool bomb_planted = false;
    long long planted_at = 0;

    keypad(map, true); int key;    
    wtimeout(map, 250);    
    while(key = wgetch(map)){
        switch(key){
            case KEY_UP:{
                last_direction = 0;
                player_pos_x--;
                if (check_collision(areas, player_pos_x, player_pos_y)){
                    player_pos_x++; break;
                }
                detect_a_coint(areas, player_pos_x, player_pos_y);
                break;
            }
            case KEY_DOWN:{
                last_direction = 1;
                player_pos_x++;
                if (check_collision(areas, player_pos_x, player_pos_y)){
                    player_pos_x--; break;
                }
                detect_a_coint(areas, player_pos_x, player_pos_y);
                break;
            }
            case KEY_LEFT:{
                last_direction = 2;
                player_pos_y--;
                if (check_collision(areas, player_pos_x, player_pos_y)){
                    player_pos_y++; break;
                }
                detect_a_coint(areas, player_pos_x, player_pos_y);
                break;
            }
            case KEY_RIGHT:{
                last_direction = 3;
                player_pos_y++;
                if (check_collision(areas, player_pos_x, player_pos_y)){
                    player_pos_y--; break;
                }
                detect_a_coint(areas, player_pos_x, player_pos_y);
                break;
            }
            case KEY_BACKSPACE:{
                if(bomb_planted == false)
                    bomb_planted = plant_a_bomb(map, areas, last_direction, player_pos_x, player_pos_y);
                    planted_at = time(NULL);
            }
            default:
                break;
        }
        areas[previous_player_pos_x][previous_player_pos_y] = 0; //player leaving area, leaves it with value 0
        areas[player_pos_x][player_pos_y] = 2; //updating players position
        draw_player(map, previous_player_pos_x, previous_player_pos_y, player_pos_x, player_pos_y);
        previous_player_pos_x = player_pos_x; previous_player_pos_y = player_pos_y;

        now = (long long)time(NULL); //calculating time left in the game
        update_time(game_time - (int)(now - current));
        if(bomb_planted)// if bomb has been planted, count the time to exploion
            bomb_planted = bomb_countdown(planted_at, now);
    }
}