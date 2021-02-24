//Konrad Maciejczyk, 2020
/*Here are written functions that controls all game mechanics i.a: player's movements, collision and coins detection, pause menu, 
time, health, score and level number update functions*/
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

extern int score; extern int game_time; extern void update_score(); extern WINDOW *hud; extern int health;
extern int map_num;

void update_score(){
   wattron(hud, COLOR_PAIR(3));
   mvwprintw(hud, 18, 12, "%i", score);
   wattroff(hud, COLOR_PAIR(3));
   wrefresh(hud);   
}

bool pause_menu(){
    WINDOW *pause_menu = newwin(10, 22, 30, 13);
    wborder(pause_menu, '*', '*', '*', '*', '*', '*', '*', '*');
    wattron(pause_menu, A_BOLD); wattron(pause_menu, A_BLINK);
    mvwprintw(pause_menu, 1, 8, "PAUSED");
    wattroff(pause_menu, A_BOLD); wattroff(pause_menu, A_BLINK);
    wrefresh(pause_menu); refresh();

    keypad(pause_menu, true);
    char P_MENU_OPTIONS[2][10] = {"Resume", "Quit Game"};

    int highlight = 0, choice;

    while(1){
        for(int i=0; i<2; i++){
            if(i == highlight){
                wattron(pause_menu, A_REVERSE);
            }
            mvwprintw(pause_menu, i+4, 7, P_MENU_OPTIONS[i]);
            wattroff(pause_menu, A_REVERSE);
        }
        choice = wgetch(pause_menu);

        switch(choice){
            case KEY_UP:
                highlight--;
                if(highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == 2)
                    highlight = 1;
                break;
            default:
                break;
        } 

        if (choice == 10){
            break;
        }       
    }

    switch(highlight){
        case 0:
            for(int i=0; i<10; i++){
                for(int j=0; j<22; j++){
                    mvwprintw(pause_menu, i, j, " ");
                }
            }
            wrefresh(pause_menu);
            delwin(pause_menu); refresh(); return true;
        case 1:
            return false;
    }
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

void update_health(){
    mvwprintw(hud, 12, 13, "          "); wmove(hud, 12, 13);
    wattron(hud, COLOR_PAIR(2)); wattron(hud, A_BOLD);
    for(int i=0; i<health; i++){
        wprintw(hud, "\u2665 ");
    }
    wattroff(hud, COLOR_PAIR(2)); wattroff(hud, A_BOLD);
}

void draw_player(WINDOW *map, int prev_pos_x, int prev_pos_y, int pos_x, int pos_y){
    mvwprintw(map, prev_pos_x+1, prev_pos_y+2, " ");

    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    wattron(map, COLOR_PAIR(5)); wattron(map, A_BOLD);
    mvwprintw(map, pos_x+1, pos_y+2,"&");
    wattroff(map, COLOR_PAIR(5)); wattroff(map, A_BOLD);
    wrefresh(map);
}

bool is_level_finished(int areas[40][80], int pos_x, int pos_y){
    if(areas[pos_x][pos_y] == 3){
        return true;
    }else{
        return false;
    }    
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

bool plant_a_bomb(WINDOW *map, int areas[40][80], int direction, int pos_x, int pos_y, int *bomb_pos_x, int *bomb_pos_y){
    init_pair(2, COLOR_RED, COLOR_BLACK);
    wattron(map, COLOR_PAIR(2));
    bool aux;
    switch (direction){
    case 0: //up
        if(!check_collision(areas, pos_x - 1, pos_y)){
            mvwprintw(map, pos_x, pos_y + 2, "\u2605");
            areas[pos_x-1][pos_y] = 6; aux = true;
            *bomb_pos_x = pos_x-1; *bomb_pos_y = pos_y;
        } break;
    case 1: //down
        if(!check_collision(areas, pos_x + 1, pos_y)){
            mvwprintw(map, pos_x + 2, pos_y + 2, "\u2605");
            areas[pos_x+1][pos_y] = 6; aux = true;
            *bomb_pos_x = pos_x+1; *bomb_pos_y = pos_y;
        } break;
    case 2: //left
        if(!check_collision(areas, pos_x, pos_y - 1)){
            mvwprintw(map, pos_x+1, pos_y + 1, "\u2605");
            areas[pos_x][pos_y-1] = 6; aux = true;
            *bomb_pos_x = pos_x; *bomb_pos_y = pos_y-1;
        } break;
    case 3: //right
        if(!check_collision(areas, pos_x, pos_y + 1)){
            mvwprintw(map, pos_x+1, pos_y + 3, "\u2605");
            areas[pos_x][pos_y+1] = 6; aux = true;
            *bomb_pos_x = pos_x; *bomb_pos_y = pos_y+1;
        } break;
    }
    wrefresh(map);
    wattroff(map, COLOR_PAIR(2));
    return aux;   
}

void detect_a_coint(int areas[40][80], int pos_x, int pos_y){
    if (areas[pos_x][pos_y] == 4){ //ordinary coin
        score += 10;
        update_score();
    }else if(areas[pos_x][pos_y] == 5){ //extraordinary coin
        score += 50;
        update_score();
    }
}

void bomb_explode(WINDOW *map, int areas[40][80], int mask[3][5], int bomb_pos_x, int bomb_pos_y){
    bomb_pos_x = bomb_pos_x == 0 ? 1:bomb_pos_x; //preventing against destroying borders of a map
    bomb_pos_x = bomb_pos_x == 40 ? 39:bomb_pos_x;
    bomb_pos_y = bomb_pos_y < 2 ? 2:bomb_pos_y;
    bomb_pos_y = bomb_pos_y > 78 ? 78:bomb_pos_y;

    for(int i=0; i<3; i++){
        for(int j=0; j<5; j++){
            if(mask[i][j] == 1){
                mvwprintw(map, bomb_pos_x + i, bomb_pos_y + j, " "); wrefresh(map);
                if(areas[bomb_pos_x - 1 + i][bomb_pos_y - 2 + j] == 2){
                    health -= 1; update_health();
                    continue;
                }
                areas[bomb_pos_x - 1 + i][bomb_pos_y - 2 + j] = 0;
            }
        }
    }
}

bool bomb_countdown(WINDOW *map, long long planted_at, long long now, int areas[40][80], int bomb_pos_x, int bomb_pos_y){;
    if(now - planted_at >= 2){
        int mask[3][5] = {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}}; //mask for explosion range
        bomb_explode(map, areas, mask, bomb_pos_x, bomb_pos_y);        
        return false;    
    }return true;
}

bool player_movement(WINDOW *map, int areas[40][80], int *start_pos){
    int player_pos_x = 6; int player_pos_y = *start_pos;
    int previous_player_pos_x = player_pos_x; int previous_player_pos_y = player_pos_y;
    int last_direction = 0; //determinates a direction player currently is pointing at    
   
    long long current = (long long)time(NULL); //getting time from OS for game timer
    long long now = current;
    int time_left = game_time;
    
    bool is_bomb_planted = false;
    long long planted_at = 0;
    int bomb_pos_x, bomb_pos_y;

    keypad(map, true); int key;    
    wtimeout(map, 250);    
    while(health > -1 && time_left > -1){
        key = wgetch(map);
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
                if(is_bomb_planted == false){
                    is_bomb_planted = plant_a_bomb(map, areas, last_direction, player_pos_x, player_pos_y, &bomb_pos_x, &bomb_pos_y);
                    planted_at = time(NULL);
                }break;
            }
            case KEY_HOME:
                game_time = time_left;
                //current = time(NULL); now = current;
                if(pause_menu()){
                    time_left = game_time;
                    current = time(NULL); now = current;
                    break;
                }else{
                    return false;
                }
            default:
                break;
        }
        
        if(is_level_finished(areas, player_pos_x, player_pos_y)){
            game_time = time_left; map_num++;
            return true;
        }    
        areas[previous_player_pos_x][previous_player_pos_y] = 0; //player leaving area, leaves it with value 0
        areas[player_pos_x][player_pos_y] = 2; //updating players position
        draw_player(map, previous_player_pos_x, previous_player_pos_y, player_pos_x, player_pos_y);
        previous_player_pos_x = player_pos_x; previous_player_pos_y = player_pos_y;

        now = (long long)time(NULL); //calculating time left in the game
        time_left = game_time - (int)(now - current);
        update_time(time_left);
        if(is_bomb_planted)// if bomb has been planted, count the time to exploion
            is_bomb_planted = bomb_countdown(map, planted_at, now, areas, bomb_pos_x, bomb_pos_y);
    }
    delwin(map); delwin(hud);
    return false; 
}
