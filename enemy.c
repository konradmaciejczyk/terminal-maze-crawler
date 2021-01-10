//Konrad Maciejczyk, 2020
#include <ncurses.h>

struct sNode{
    bool isobstacle;
    bool isvisited;
    int globalGoal;
    int localGoal;
    int x;
    int y;
    struct sNode *neighbours[4];
    struct sNode *parent;
};
struct sNode nodes[40 * 80];

void initialize_nodes(int areas[40][80]){
    int aux = 0;
    for(int i=0; i<40; i++){
        for(int j=0; j<80; j++){
            nodes[aux].x = i; nodes[aux].y = j;
            if(areas[i][j] == 1){
                nodes[aux].isobstacle = true;
            }
            nodes[aux].parent = NULL;
            nodes[aux].neighbours[0] = NULL; nodes[aux].neighbours[1] = NULL; nodes[aux].neighbours[2] = NULL; nodes[aux].neighbours[3] = NULL;
            nodes[aux].localGoal = INT32_MAX; nodes[aux].globalGoal = INT32_MAX;
            aux++;           
        }
    }
}