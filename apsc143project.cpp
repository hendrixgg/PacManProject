// APSC 143 Engineering Programming Project Starter Code
// Feel free to change anything about this file, but do not remove "#include "colours.h".

// Make sure to include all relevant libraries
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
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

void initGame(const char *mapFilePath, char ***map, int rows, int cols, int pacManPos[2], int ghostPos[2][2]){
    FILE* mapFile = fopen(mapFilePath, "r");
    *map = (char **)malloc(rows*sizeof(char*));
    int numGhosts = 0;
    for(int i = 0; i < rows; ++i) {
        (*map)[i] = (char *) malloc(cols * sizeof(char));
        for(int j = 0; j < cols; ++j){
            fscanf(mapFile, "%c ", &(*map)[i][j]);
            if((*map)[i][j] == PACMAN) {
                (*map)[i][j] = EMPTY;
                pacManPos[0] = i, pacManPos[1] = j;
            }else if((*map)[i][j] == GHOST){
                (*map)[i][j] = DOT;
                ghostPos[numGhosts][0] = i, ghostPos[numGhosts][1] = j, ++numGhosts;
            }
        }
    }
}

// TODO: add in colors
void printMap(char **map, int rows, int cols, int pacManPos[2], int ghostPos[2][2]){
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
int winCheck(/*parameters*/){
    return 0;
}
int loseCheck(/*parameters*/){
    return 0;
}
int isWall(/*parameters*/){
    return 0;
}

int main() {
    int pacManPos[2], ghostPos[2][2];
    char **map, key = 0;
    // load the map array (9 rows, 9 cols) of characters, and get initial PacMan and Ghost positions
    initGame("map.txt", &map, ROWS, COLS, pacManPos, ghostPos);

    printf("press 'q' or esc to exit.\n");
    while(true){
        system("CLS");
        printMap(map, ROWS, COLS, pacManPos, ghostPos);
        // input
        if(key == ESC || key == 'q' || key == 'Q')
            break;
        printf("input: %d, %c\n", key, key);
        key = getch();

        // TODO: move ghosts
            // array for location of ghosts
            // determine direction of movement (line of sight, or random)

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
