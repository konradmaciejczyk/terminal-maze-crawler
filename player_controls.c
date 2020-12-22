//Konrad Maciejczyk, 2020
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <wchar.h>

void draw_player(WINDOW *map, int prev_pos_x, int prev_pos_y, int pos_x, int pos_y){
    mvwprintw(map, prev_pos_y, prev_pos_x, " ");

    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    wattron(map, COLOR_PAIR(5)); wattron(map, A_BOLD);
    mvwprintw(map, pos_y, pos_x, "&");
    wattroff(map, COLOR_PAIR(5)); wattroff(map, A_BOLD);
    wrefresh(map);
}
int check_collision(int areas[40][80], int pos_x, int pos_y){
    if(areas[pos_x][pos_y]==1){
        return 1;
    }else{
        return 0;
    }
}
void player_movement(WINDOW *map, int areas[40][80], int *start){
    keypad(map, true);
    int player_pos_x = *start; int player_pos_y = 6;
    int previous_player_pos_x = player_pos_x; int previous_player_pos_y = player_pos_y;
    while(1){
        int key = wgetch(map);

        switch(key){
            case KEY_UP:
                player_pos_y--;
                if (!check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_y++;
                break;
            case KEY_DOWN:
                player_pos_y++;
                if (!check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_y--;
                break;
            case KEY_LEFT:
                player_pos_x--;
                if (!check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_x++;
                break;
            case KEY_RIGHT:
                player_pos_x++;
                if (!check_collision(areas, player_pos_x, player_pos_y))
                    player_pos_x--;
                break;
            default:
                break;
        }
        printw("(%i, %i| %i)", player_pos_x, player_pos_y, areas[player_pos_x][player_pos_y]); refresh();

        areas[previous_player_pos_x][previous_player_pos_y] = 0;
        areas[player_pos_x][player_pos_y] = 2;
        draw_player(map, previous_player_pos_x, previous_player_pos_y, player_pos_x, player_pos_y);
        previous_player_pos_x = player_pos_x; previous_player_pos_y = player_pos_y;
        
    }
}