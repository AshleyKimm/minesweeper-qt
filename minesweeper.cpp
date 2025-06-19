#include <iostream>
#include "minesweeper_lib.h"

char *createBoard(size_t xdim, size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, size_t xdim, size_t ydim);
void hideBoard(char *board, size_t xdim, size_t ydim);
int mark(char *board, size_t xdim, size_t ydim, size_t xloc, size_t yloc);
bool isGameWon(char *board, size_t xdim, size_t ydim);
void computeNeighbors(char *board, size_t xdim, size_t ydim);
int reveal(char *board, size_t xdim, size_t ydim, size_t xloc, size_t yloc);


char *createBoard(size_t xdim, size_t ydim) {
    char *array{new char[xdim * ydim]{}};
    return array;
}
void cleanBoard(char *board) {
    delete[] board;
    board = nullptr;
}
void printBoard(char *board, size_t xdim, size_t ydim) {
    for (size_t i{}; i < (xdim * ydim); ++i) {
        if ((board[i] & markedBit()) == markedBit()) std::cout << 'M';
        else if ((board[i] & hiddenBit()) == hiddenBit()) std::cout << '*';
        else std::cout << (board[i] & valueMask());
        if ((i + 1) % xdim == 0) std::cout << std::endl;
    }
}
void hideBoard(char *board, size_t xdim, size_t ydim) {
    for (size_t i{}; i < (xdim * ydim); ++i) {
        board[i] = board[i] | hiddenBit();
    } 
}
int mark(char *board, size_t xdim, size_t ydim, size_t xloc, size_t yloc) {
    size_t i{(yloc * xdim) + xloc};
    if (((board[i] & hiddenBit()) != hiddenBit()) && ((board[i] & markedBit()) != markedBit())) return 2;
    board[i] ^= markedBit();
    return 0;
}
bool isGameWon(char *board, size_t xdim, size_t ydim) {
    for (int i{}; i < xdim * ydim; ++i) {
        if (((board[i] & hiddenBit()) == hiddenBit()) && ((board[i] & valueMask()) != 9)) return false;
    }
    return true;
}
void computeNeighbors(char *board, size_t xdim, size_t ydim) {
   int xint{};
    while (xint < xdim) ++xint;
    for (int i{}; i < xdim * ydim; ++i) {
        if ((board[i] & valueMask()) == 9) {
            for (int y{-1}; y <= 1; ++y) {
                for (int x{-1}; x <= 1; ++x) {
                    int check{((y * xint) + i + x)};
                    if ((check >= (((i / xdim) + y) * xdim)) && (check <= ((((i / xdim) + y) * xdim) + (xdim - 1))) && (i != check) && ((board[check] & valueMask()) != 9)) {
                        board[check] ++;
                    }
                }
            }
        }
    }
}
int reveal(char *board, size_t xdim, size_t ydim, size_t xloc, size_t yloc) {
    int xint{}, yl{}, xl{};
    while (xint < xdim) ++xint;
    while (yl < yloc) ++yl;
    while (xl < xloc) ++xl;
    int i{(yl * xint) + xl};
    if ((board[i] & markedBit()) == markedBit()) return 1;
    if ((board[i] & hiddenBit()) != hiddenBit()) return 2;
   
   
    board[i] ^= hiddenBit();
    if ((board[i] & valueMask()) == 9) return 9; 
    if ((board[i] & valueMask()) == 0) {
    
        for (int y{-1}; y <= 1; ++y) {
            for (int x{-1}; x <= 1; ++x) {
                int check{((y * xint) + i + x)};
                if (((board[check] & hiddenBit()) == hiddenBit()) && (check >= (((i / xdim) + y) * xdim)) && (check <= ((((i / xdim) + y) * xdim) + (xdim - 1)))) board[check] ^= hiddenBit();
            }
        }
    }    
    return 0;
}


