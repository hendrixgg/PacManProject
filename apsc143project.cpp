// APSC 143 Engineering Programming Project Starter Code
// Feel free to change anything about this file, but do not remove "#include "colours.h"".

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

// game constants
#define COLS 11
#define ROWS 11
#define NUM_GHOSTS 2

// returns 1 if the tile specified is a wall tile or out of bounds, 0 if not.
int isWall(char **map, const int row, const int col){
    return row < 0 || ROWS <= row || col < 0 || COLS <= col || map[row][col] == WALL;
}

// returns 1 if the tile specified contains a ghost, 0 if not
int isGhost(const int ghostPos[NUM_GHOSTS][2], const int row, const int col){
    for(int i = 0; i < NUM_GHOSTS; ++i)
        if(row == ghostPos[i][0] && col == ghostPos[i][1])
            return 1;
    return 0;
}

// initializes the map and the entity positions
// returns 1 if successful, 0 if the mapfile could not be opened, and -1 if the end of file was reached before the expected amount of input had been read
int initGame(const char *mapFilePath, char ***map, const int rows, const int cols, int pacManPos[2], int ghostPos[2][2], int *dots){
    FILE* mapFile = fopen(mapFilePath, "r");
    if(mapFile == NULL){
        printf("error opening %s: No such file or directory\n", mapFilePath);
        fclose(mapFile);
        return 0;
    }
    *map = (char **)malloc(rows*sizeof(char*)), *dots = 0;
    for(int i = 0, numGhosts = 0; i < rows; ++i) {
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
    return 1;
}

// prints the map and entities with different colours to the console
void printMap(char **map, const int rows, const int cols, const int pacManPos[2], const int ghostPos[2][2]){
    for(int i = 0; i < rows; ++i){
        colourChange(BLUE);
        printf("%c", map[i][0]);
        for(int j = 1; j < cols; ++j){
            char c = map[i][j];
            if(isGhost(ghostPos, i, j)){
                c = GHOST;
                colourChange(PINK);
            } else if(i == pacManPos[0] && j == pacManPos[1]){
                c = PACMAN;
                colourChange(YELLOW);
            } else if (c == WALL)
                colourChange(BLUE);
            else // if c == DOT
                colourChange(WHITE);
            printf("  %c", c);
        }
        puts("");
    }
    colourChange(WHITE);
}

// compute the minimum distance to pac man from position on the map (i, j) using depth-first-search
// returns a 2d array of ints {dist, direction index}
int distToPacMan(char **map, int vis[ROWS][COLS], int i, int j, const int pacManPos[2], int dirs[4][2]){
    if(isWall(map, i, j) || vis[i][j] == 1)
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

// move ghost in direction of the shortest path to pac man
void moveGhost(char **map, const int allGhosts[NUM_GHOSTS][2], const int pacManPos[2], int ghostPos[2]){
    int minDist = 1e9, dirIdx = 0, dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // {up, down, left, right}
    int vis[ROWS][COLS];

    for(int i = 0; i < 4; ++i) {
        if(isWall(map, ghostPos[0] + dirs[i][0], ghostPos[1] + dirs[i][1]) || isGhost(allGhosts, ghostPos[0] + dirs[i][0], ghostPos[1] + dirs[i][1])) continue;
        int dist = distToPacMan(map, vis, ghostPos[0] + dirs[i][0], ghostPos[1] + dirs[i][1], pacManPos, dirs);
        if(dist < minDist)
            dirIdx = i, minDist = dist;
    }

    ghostPos[0] += dirs[dirIdx][0], ghostPos[1] += dirs[dirIdx][1];
}

// Changes PacMan's position based on key input if the new position is not a wall.
void movePacman(char key, char **map, int pacManPos[2]){
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

// Remove dot from map if PacMan is on a dot
// returns 1 if dot removed, 0 otherwise
int removeDot(char **map, int pacManPos[2]){
    if(map[pacManPos[0]][pacManPos[1]] == DOT){
        map[pacManPos[0]][pacManPos[1]] = EMPTY;
        return 1;
    }
    return 0;
}

// Check if PacMan has won if the dots remaining are equal to zero.
// returns 1 if the game is won, 0 otherwise
int winCheck(int dotsRemaining){
    return dotsRemaining == 0;
}

// If Pacman hits a ghost, he loses.
// returns 1 if the game is lost, 0 otherwise
int loseCheck(int pacManPos[2], int ghostPos[NUM_GHOSTS][2]){
    // differences in position a ghost can have from pac man that are losing
    int dirs[5][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {0, 0}}; // {up, down, left, right, contact}
    char strDirs[5][10] = {"Above", "Below", "Left", "Right", "Contact"};
    for(int i = 0; i < 5; ++i){
        if(isGhost(ghostPos, pacManPos[0] + dirs[i][0], pacManPos[1] + dirs[i][1])){
            // puts(strDirs[i]);
            return 1;
        }
    }
    return 0;
}

// receives input from the user and only returns the key if it would have an effect on the game
char input(){
    char key;
    do{
        key = getch();
    }while(!(key == UP || key == DOWN || key == LEFT || key == RIGHT || key == ESC || key == 'q' || key == 'Q'));
    // printf("input: %d, %c\n", key, key);
    return key;
}

int main() {
    int pacManPos[2], ghostPos[2][2], dotsRemaining;
    char **map, key = 0;
    // load the map array (11 rows, 11 cols) of characters, and get initial PacMan and Ghost positions
    int status = initGame("../map.txt", &map, ROWS, COLS, pacManPos, ghostPos, &dotsRemaining);

    while(status > 0){
        printf("press 'q' or esc to exit the game\n");
        printMap(map, ROWS, COLS, pacManPos, ghostPos);

        // input
        key = input();
        if(key == ESC || key == 'q' || key == 'Q')
            break;

        // move ghosts
        // determine direction of movement (line of sight, or random, or Breadth-First-Search from ghost to pacman)
        // printf("ghosts: (%d, %d) (%d, %d)\n", ghostPos[0][0], ghostPos[0][1], ghostPos[1][0], ghostPos[1][1]);
        // printf("pac man: (%d, %d)\n", pacManPos[0], pacManPos[1]);
        for(int i = 0; i < NUM_GHOSTS; ++i){
            // printf("ghost %d: \n", i);
            moveGhost(map, ghostPos, pacManPos, ghostPos[i]);
        }

        // move PacMan
        movePacman(key, map, pacManPos);
        // collect a pellet if PacMan lands on one
        dotsRemaining -= removeDot(map, pacManPos);

        // clear the console
        system("CLS");

        // check if won/lost -> if yes: break the loop and print game over condition to user
        if(winCheck(dotsRemaining) || loseCheck(pacManPos, ghostPos)) 
            break;
    }

    if(key == ESC || key == 'q' || key == 'Q'){
        printf("You ended the game. Press any key to exit the game\n");
    } else if(dotsRemaining > 0){
        printMap(map, ROWS, COLS, pacManPos, ghostPos);
        // printf("pacman(%d, %d), ghosts(%d, %d), (%d, %d)\n", pacManPos[0], pacManPos[1], ghostPos[0][0], ghostPos[0][1], ghostPos[1][0], ghostPos[1][1]);
        printf("Sorry, you lose. Press any key to exit the game\n");
    } else {
        printMap(map, ROWS, COLS, pacManPos, ghostPos);
        printf("Congratulations! You win! Press any key to exit the game\n");
    }
    getch();
    free(map);
    return 0;
}
