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
#define COLS 9
#define ROWS 9

// Below are the required functions for this program, but you will need to create your own as well.
// Make sure to specify the required parameters, if any.
char ** initMap(const char *mapFilePath, int rows, int cols){
    FILE* mapFile = fopen(mapFilePath, "r");
    char **map = (char **)malloc(rows*sizeof(char*));
    for(int i = 0; i < rows; ++i) {
        map[i] = (char *) malloc(cols * sizeof(char));
        for(int j = 0; j < cols; ++j){
            fscanf(mapFile, "%c ", &map[i][j]);
        }
    }
    return map;
}
void printMap(char **map, int rows, int cols){
    for(int i = 0; i < rows; ++i){
        printf("%c ", map[i][0]);
        for(int j = 1; j < cols; ++j){
            printf(" %c ", map[i][j]);
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
    // load the map array (9 rows, 9 cols) of characters
    char **map = initMap("map.txt", ROWS, COLS);

    printf("press 'q' or esc to exit.\n");
    while(true){
        printMap(map, ROWS, COLS);
        // input
        char input = getch();
        if(input == ESC || input == 'q' || input == 'Q')
            break;
        printf("input: %d, %c\n", input, input);
    }
    printf("game over\n");
    printf("press any key to exit\n");
    getch();
    return 0;
}
