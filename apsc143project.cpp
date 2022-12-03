// Make sure to include all relevant libraries
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "colours.h"

// Constants regarding icons
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '

// Constants regarding player movement
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define START 4

// map.txt constants
#define MAP_ROWS 9
#define MAP_COLS 9
#define NUM_GHOSTS 2

// game constants
#define ROWS (MAP_ROWS+2)
#define COLS (MAP_COLS+2)
// {up, down, left, right, contact}
const int directions[5][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {0, 0}}; // Each possible direction and contact blocks contained in a list
int keyDir[UP+1]; // maps an ascii character to an index in directions

// game variables
int pacManPos[2], ghostPos[NUM_GHOSTS][2], dotsRemaining, status;
char **map;
bool vis[ROWS][COLS]{false}; // for distToPacman function

// returns 1 if the tile specified is a wall tile or out of bounds, 0 if not.
int isWall(const int row, const int col){
    return row < 0 || ROWS <= row || col < 0 || COLS <= col || map[row][col] == WALL;
}

// returns 1 if the tile specified contains a ghost, 0 if not
int isGhost(const int row, const int col){
    for(int i = 0; i < NUM_GHOSTS; ++i)
        if(row == ghostPos[i][0] && col == ghostPos[i][1])
            return 1;
    return 0;
}

// initializes the map and the entity positions
// returns 1 if successful, 0 if the mapfile could not be opened, and -1 if the end of file was reached before the expected amount of input had been read
int initGame(const char *mapFilePath){
    // File opener
    FILE* mapFile = fopen(mapFilePath, "r");
    if(mapFile == NULL){
        printf("error opening %s: No such file or directory\n", mapFilePath);
        fclose(mapFile);
        return 0;
    }

    // initialize stuff
    dotsRemaining = 0, keyDir[UP] = 0, keyDir[DOWN] = 1, keyDir[LEFT] = 2, keyDir[RIGHT] = 3;

    // allocate map memory
    map = (char **)malloc(ROWS * sizeof(char*));
    // initialize WALLs
    for(int row = 0; row < ROWS; ++row){
        map[row] = (char *) malloc(COLS * sizeof(char));
        memset(map[row], WALL, COLS * sizeof(char));
    }
    // read input from file
    for(int row = 1, numGhosts = 0; row < ROWS-1; ++row) {
        for(int col = 1; col < COLS-1; ++col){
            // read input and check for error
            if(fscanf(mapFile, "%c ", &map[row][col]) == EOF){
                printf("error reading %s: reached end of file\n", mapFilePath);
                fclose(mapFile);
                return -1;
            }
            if(map[row][col] == PACMAN) {
                map[row][col] = EMPTY;
                pacManPos[0] = row, pacManPos[1] = col;
            }else if(map[row][col] == GHOST){
                // assuming that a ghost is covering a dot
                map[row][col] = DOT, ++dotsRemaining;
                ghostPos[numGhosts][0] = row, ghostPos[numGhosts][1] = col, ++numGhosts;
            }else if(map[row][col] == DOT){
                ++dotsRemaining;
            }
        }
    }
    fclose(mapFile);
    return 1; // Everything completed initialization without any errors
}

// prints the map and entities with different colours to the console
void printMap(){
    for(int row = 0; row < ROWS; ++row){
        colourChange(BLUE);
        printf("%c", map[row][0]);
        for(int col = 1; col < COLS; ++col){
            char c = map[row][col];
            if(isGhost(row, col)){
                c = GHOST;
                colourChange(PINK);
            } else if(row == pacManPos[0] && col == pacManPos[1]){
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

// returns the minimum distance to pac man from position on the map (row, col) using depth-first-search
// dirIdx will be the direction corresponding with this shortest path
int distToPacMan(int *dirIdx, int row, int col){
    if(row == pacManPos[0] && col == pacManPos[1])
        return 0;

    vis[row][col] = true;

    int minDist = 1e9, tmpDirIdx = -1, newDirIdx = START;
    for(int d = 0; d < 4; ++d) {
        int newRow = row + directions[d][0], newCol = col + directions[d][1];
        if(isWall(newRow, newCol) || vis[newRow][newCol] || (*dirIdx == START && isGhost(newRow, newCol))) continue;
        // if this is a valid move, compute distance to pac man
        int dist = distToPacMan(&tmpDirIdx, newRow, newCol);
        if(dist < minDist)
            newDirIdx = d, minDist = dist;
    }
    *dirIdx = newDirIdx;
    vis[row][col] = false;
    return 1 + minDist;
}

// move ghosts in direction of the shortest path to pac man
void moveGhosts(){
    for(int g = 0, dirIdx = START; g < NUM_GHOSTS; ++g, dirIdx = START){
        distToPacMan(&dirIdx, ghostPos[g][0], ghostPos[g][1]);
        ghostPos[g][0] += directions[dirIdx][0], ghostPos[g][1] += directions[dirIdx][1];
    }
}

// Changes PacMan's position based on key input if the new position is not a wall.
void movePacman(int key){
    const int newRow = pacManPos[0] + directions[keyDir[key]][0], newCol = pacManPos[1] + directions[keyDir[key]][1];
    if(!isWall(newRow, newCol))
        pacManPos[0] = newRow, pacManPos[1] = newCol;
}

// Remove dot from map if PacMan is on a dot
// returns 1 if dot removed, 0 otherwise
int removeDot(){
    if(map[pacManPos[0]][pacManPos[1]] == DOT){
        map[pacManPos[0]][pacManPos[1]] = EMPTY, --dotsRemaining;
        return 1;
    }
    return 0;
}

// Check if PacMan has won if the dots remaining are equal to zero.
// returns 1 if the game is won, 0 otherwise
int winCheck(){
    removeDot();
    return dotsRemaining == 0;
}

// If Pacman hits a ghost, he loses.
// returns 1 if the game is lost, 0 otherwise
int loseCheck(){
    for(int d = 0; d < sizeof(directions)/sizeof(directions[0]); ++d)
        if(isGhost(pacManPos[0] + directions[d][0], pacManPos[1] + directions[d][1]))
            return 1;
    return 0;
}

// receives input from the user and only returns the key if it would have an effect on the game
int input(){
    int key;
    do{key = getch();}while(!(key == UP || key == DOWN || key == LEFT || key == RIGHT));
    return key;
}

int main() {
    // load the map array (11 rows, 11 cols) of characters, get initial PacMan and Ghost positions, initialize other variables
    status = initGame("../map.txt");
    while(status > 0){
        printMap();
        // check if won/lost -> if yes: break the loop and print game over condition to user
        if(loseCheck() || winCheck()) break;
        // movement
        moveGhosts(), movePacman(input());
        // clear the console
        system("CLS");
    }
    // game over message
    printf("%s Press any key to exit the game\n", (dotsRemaining > 0) ? "Sorry, you lose." : "Congratulations! You win!");
    getch();
    for(int row = 0; row < ROWS; ++row)
        free(map[row]);
    free(map);
    return 0;
}