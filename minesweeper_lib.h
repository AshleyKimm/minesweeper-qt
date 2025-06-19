#ifndef minesweeper_LIB_H
#define minesweeper_LIB_H

#include <iostream>

int main();
bool game();
void startGame(char * & board, size_t & xdim, size_t & ydim, unsigned int & numMine);
char getAction();
void actionShow(char * & board, size_t & xdim, size_t & ydim, unsigned int & numMine);
void actionMark(char * board, size_t xdim, size_t ydim);
size_t readSizeT();
size_t xdim_max();
size_t ydim_max();
char markedBit();
char hiddenBit();
char valueMask();
void spreadMine(char * board, size_t xdim, size_t ydim, unsigned int numMine);

#endif
