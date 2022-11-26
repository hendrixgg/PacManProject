// APSC 143 Engineering Programming Project Starter Code
// Feel free to change anything about this file, but do not remove "#include "colours.h".

// Make sure to include all relevant libraries
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "colours.h"

// You don't need to use these, but they help make your code look more organized
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'
#define ESC 27
#define COLS 11
#define ROWS 11

// returns the pointer to the minimum element in the array
// assumes that size > 0
int *min(int *arr, size_t size){
    int *min = arr;
    for(size_t i = 1; i < size; ++i){
        if(arr[i] < *min)
            min = arr + i;
    }
    return min;
}

int initGame(const char *mapFilePath, char ***map, int rows, int cols, int pacManPos[2], int ghostPos[2][2]){
    FILE* mapFile = fopen(mapFilePath, "r");
    if(mapFile == NULL){
        printf("error opening %s: No such file or directory\n", mapFilePath);
        fclose(mapFile);
        return 1;
    }
    *map = (char **)malloc(rows*sizeof(char*));
    int numGhosts = 0;
    for(int i = 0; i < rows; ++i) {
        (*map)[i] = (char *) malloc(cols * sizeof(char));
        for(int j = 0; j < cols; ++j){
            // read input and check for error
            if(fscanf(mapFile, "%c ", &(*map)[i][j]) == EOF){
                printf("error reading %s: reached end of file\n", mapFilePath);
                fclose(mapFile);
                return -1;
            }
            if((*map)[i][j] == PACMAN) {
                (*map)[i][j] = EMPTY;
                pacManPos[0] = i, pacManPos[1] = j;
            }else if((*map)[i][j] == GHOST){
                // assuming that a ghost is covering a dot
                (*map)[i][j] = DOT;
                ghostPos[numGhosts][0] = i, ghostPos[numGhosts][1] = j, ++numGhosts;
            }
        }
    }
    fclose(mapFile);
    return 0;
}

// TODO: add in colors
void printMap(char **map, const int rows, const int cols, const int pacManPos[2], const int ghostPos[2][2]){
    for(int i = 0; i < rows; ++i){
        printf("%c ", map[i][0]);
        for(int j = 1; j < cols; ++j){
            char c = map[i][j];
            if(i == pacManPos[0] && j == pacManPos[1])
                c = PACMAN;
            for(int k = 0; k < 2; ++k)
                if(i == ghostPos[k][0] && j == ghostPos[k][1])
                    c = GHOST;
            printf(" %c ", c);
        }
        puts("");
    }
}

// move ghost in direction of shortest path to pac man
void moveGhost(char **map, int ghostPos[2], int pacManPos[2]){
    int dir[2];
    int vis[ROWS][COLS];
}

//returns 1 if the nearest tile in the specified direction is a wall tile or out of bounds, and 0 if not.
int isWall(char **map, int i, int j){
    return map[i][j] == WALL;
}

// comput the minimum distance to pac man from position on the map (i, j) using breadth-first-search
int distToPacMan(char **map, int vis[ROWS][COLS], int i, int j, int pacManPos[2]){
    if(vis[i][j] == 1 || isWall(map, i, j))
        return 1e5;
    if(i == pacManPos[0] && j == pacManPos[1])
        return 0;
    
    vis[i][j] = 1;

    int dist[4];
    // move up
    dist[0] = distToPacMan(map, vis, i + 1, j, pacManPos);
    // move down
    dist[1] = distToPacMan(map, vis, i - 1, j, pacManPos);
    // move left
    dist[2] = distToPacMan(map, vis, i, j - 1, pacManPos);
    // move right
    dist[3] = distToPacMan(map, vis, i, j + 1, pacManPos);

    vis[i][j] = 0;
    return 1 + *min(dist, 4);
}

int winCheck(/*parameters*/){
    return 0;
}
int loseCheck(/*parameters*/){
    return 0;
}


int main() {
    int pacManPos[2], ghostPos[2][2];
    char **map, key = 0;
    // load the map array (9 rows, 9 cols) of characters, and get initial PacMan and Ghost positions
    int status = initGame("../map.txt", &map, ROWS, COLS, pacManPos, ghostPos);

    printf("press 'q' or esc to exit.\n");
    while(status == 0){
        system("CLS");
        printMap(map, ROWS, COLS, pacManPos, ghostPos);
        // input
        if(key == ESC || key == 'q' || key == 'Q')
            break;
        printf("input: %d, %c\n", key, key);
        key = getch();

        // TODO: move ghosts
            // determine direction of movement (line of sight, or random, or Breadth-First-Search from ghost to pacman)

        // TODO: move PacMan
            // check if the input was valid
            // collect a pellet if PacMan lands on one

        // TODO: check if won/lost -> if yes: break the loop and print game over condition to user
    }
    printf("game over\n");
    printf("press any key to exit\n");
    getch();
    free(map);
    return 0;
}
