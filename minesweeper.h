#ifndef minesweeper_H
#define minesweeper_H

char * createBoard(size_t xdim,  size_t ydim);
void computeNeighbors(char * board, size_t xdim, size_t ydim);
void hideBoard(char * board, size_t xdim, size_t ydim);
void cleanBoard(char * board);
void printBoard(char * board, size_t xdim, size_t ydim);
int reveal(char * board, size_t xdim, size_t ydim, size_t xloc, size_t yloc);
int mark(char * board, size_t xdim, size_t ydim, size_t xloc, size_t yloc);
bool isGameWon(char * board, size_t xdim, size_t ydim);

#endif
