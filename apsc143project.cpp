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

int initGame(const char *mapFilePath, char ***map, int rows, int cols, int pacManPos[2], int ghostPos[2][2], int *dots){
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
                (*map)[i][j] = DOT, ++*dots;
                ghostPos[numGhosts][0] = i, ghostPos[numGhosts][1] = j, ++numGhosts;
            }else if((*map)[i][j] == DOT){
                ++*dots;
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

//returns 1 if the nearest tile in the specified direction is a wall tile or out of bounds, and 0 if not.
int isWall(char **map, int i, int j){
    return map[i][j] == WALL;
}

// compute the minimum distance to pac man from position on the map (i, j) using breadth-first-search
// returns a 2d array of ints {dist, direction index}
int distToPacMan(char **map, int vis[ROWS][COLS], int i, int j, int pacManPos[2], int dirs[4][2]){
    if(isWall(map, i, j) || vis[i][j] == 1 || map[i][j] == GHOST)
        return 1e5;
    if(i == pacManPos[0] && j == pacManPos[1])
        return 0;

    vis[i][j] = 1;

    int minDist = 1e9;
    for(int k = 0; k < 4; ++k) {
        int dist = distToPacMan(map, vis, i + dirs[k][0], j + dirs[k][1], pacManPos, dirs);
        if(dist < minDist)
            minDist = dist;
    }

    vis[i][j] = 0;
    return 1 + minDist;
}

// move ghost in direction of shortest path to pac man
void moveGhost(char **map, int ghostPos[2], int pacManPos[2]){
    int minDist = 1e9, dirIdx = 0, dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // {up, down, left, right}
    int vis[ROWS][COLS];

    for(int i = 0; i < 4; ++i) {
        if(isWall(map, ghostPos[0] + dirs[i][0], ghostPos[1] + dirs[i][1])) continue;
        int dist = distToPacMan(map, vis, ghostPos[0] + dirs[i][0], ghostPos[1] + dirs[i][1], pacManPos, dirs);
        if(dist < minDist)
            dirIdx = i, minDist = dist;
    }
    printf("direction: %d\n", dirIdx);
    ghostPos[0] += dirs[dirIdx][0], ghostPos[1] += dirs[dirIdx][1];
}

//Remove dot from map if PacMan is on a dot.
int removeDot(char **map, int pacManPos[2]){
    if(map[pacManPos[0]][pacManPos[1]] == DOT){
        map[pacManPos[0]][pacManPos[1]] = EMPTY;
        return 1;
    }
    return 0;
}

int winCheck(/*parameters*/){
    return 0;
}
int loseCheck(/*parameters*/){
    return 0;
}

//Changes PacMan's position based on key input if the new position is valid.
void movePacman(char key, char **map, int pacManPos[2], int *dots){
    switch (key){
        case UP: {
            if(isWall(map,pacManPos[0]-1,pacManPos[1])){
                break;
            }else{
                --pacManPos[0];
            }
            break;
        }
        case DOWN: {
            if(isWall(map,pacManPos[0]+1,pacManPos[1])){
                break;
            }else{
                ++pacManPos[0];
            }
            break;
        }
        case RIGHT: {
            if(isWall(map,pacManPos[0],pacManPos[1]+1)){
                break;
            }else{
                ++pacManPos[1];
            }
            break;
        }
        case LEFT: {
            if(isWall(map,pacManPos[0],pacManPos[1]-1)){
                break;
            }else{
                --pacManPos[1];
            }
            break;
        }
    }
}


int main() {
    int pacManPos[2], ghostPos[2][2], dotsRemaining;
    char **map, key = 0;
    // load the map array (9 rows, 9 cols) of characters, and get initial PacMan and Ghost positions
    int status = initGame("../map.txt", &map, ROWS, COLS, pacManPos, ghostPos, &dotsRemaining);

    printf("press 'q' or esc to exit.\n");
    while(status == 0){
        system("CLS");
        printMap(map, ROWS, COLS, pacManPos, ghostPos);
        // input
        if(key == ESC || key == 'q' || key == 'Q')
            break;
        printf("input: %d, %c\n", key, key);
        key = getch();

        // move PacMan
        movePacman(key, map, pacManPos, &dotsRemaining);
        // collect a pellet if PacMan lands on one
        dotsRemaining -= removeDot(map, pacManPos);
        printf("ghosts: (%d, %d) (%d, %d)\n", ghostPos[0][0], ghostPos[0][1], ghostPos[1][0], ghostPos[1][1]);
        // TODO: move ghosts
        for(int i = 0; i < 2; ++i)
            moveGhost(map, ghostPos[i], pacManPos);
        // determine direction of movement (line of sight, or random, or Breadth-First-Search from ghost to pacman)

        // TODO: check if won/lost -> if yes: break the loop and print game over condition to user
    }
    printf("game over\n");
    printf("press any key to exit\n");
    getch();
    free(map);
    return 0;
}
