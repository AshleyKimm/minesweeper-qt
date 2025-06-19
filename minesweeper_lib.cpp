
#include <cstdlib>
#include <iostream>
#include <cctype>
#include <ctime>      // for time(NULL)
#include <cstdlib>
#include "minesweeper_lib.h"
#include "minesweeper.h"

int main_old()
{
  srand(static_cast<unsigned int>(time(NULL)));
  game();
  return 0;
}

bool game()
{
  size_t xdim {}, ydim {};
  unsigned int numMine {};
  char * gameBoard {NULL};

  startGame(gameBoard, xdim, ydim, numMine);

  char currAction {};
  while(currAction != 'Q')
  {
    switch (currAction)
    {
      case 'S' :  // show
      {
        actionShow(gameBoard, xdim, ydim, numMine);
        break;
      }
      case 'M' :  // mark
      {
        actionMark(gameBoard, xdim, ydim);
        break;
      }
      case 'R' :  // restart
      {
        std::cout << "Restarting the game." << std::endl;
        startGame(gameBoard, xdim, ydim, numMine);
        break;
      }
    }
    printBoard(gameBoard, xdim, ydim);
    if (isGameWon(gameBoard, xdim, ydim))
    {
      std::cout << "You have revealed all the cells without opening a mined cell!" << std::endl;
      std::cout << "YOU WON!!!" << std::endl;
      for (size_t reveal_row {0}; reveal_row < ydim; reveal_row++)
      {
        for (size_t reveal_col {0}; reveal_col < xdim; reveal_col++)
        {
          gameBoard[reveal_row*xdim + reveal_col] = (gameBoard[reveal_row*xdim + reveal_col] & valueMask());
        }
      }
      printBoard(gameBoard, xdim, ydim);
      std::cout << "Resetting the game board." << std::endl;
      startGame(gameBoard, xdim, ydim, numMine);
      printBoard(gameBoard, xdim, ydim);
    }
    currAction = getAction();
  }

  cleanBoard(gameBoard);
  return true;
}

void startGame(char * & board, size_t & xdim, size_t & ydim, unsigned int & numMine)
{
  cleanBoard(board);
  board = createBoard(xdim, ydim);
  spreadMine(board, xdim, ydim, numMine);
  computeNeighbors(board, xdim, ydim);
  hideBoard(board, xdim, ydim);
}

char getAction()
{
  char action {0};

  std::cout << "Please enter the action ([S]how, [M]ark, [R]estart, [Q]uit): ";
  std::cin >> action;

  if (islower(action))
    action = toupper(action);

  return action;
}

void actionShow(char * & board, size_t & xdim, size_t & ydim, unsigned int & numMine)
{
  size_t reveal_x {0};
  size_t reveal_y {0};
  std::cout << "Please enter the x location to show: ";
  std::cin >> reveal_x;
  std::cout << "Please enter the y location to show: ";
  std::cin >> reveal_y;

  if (reveal_x >= xdim || reveal_y >= ydim)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (board[xdim*reveal_y + reveal_x] & markedBit())
  {
    std::cout << "Location is marked, and therefore cannot be revealed." << std::endl;
    std::cout << "Use Mark on location to unmark." << std::endl;
  }
  else if (reveal(board, xdim, ydim, reveal_x, reveal_y) == 9)
  {
    std::cout << "You disturbed a goose! Your game has ended." << std::endl;
    printBoard(board, xdim, ydim);
    std::cout << "Starting a new game." << std::endl;
    startGame(board, xdim, ydim, numMine);
  }
}

void actionMark(char * board, size_t xdim, size_t ydim)
{
  size_t mark_x {0};
  size_t mark_y {0};
  std::cout << "Please enter the x location to mark: ";
  std::cin >> mark_x;
  std::cout << "Please enter the y location to mark: ";
  std::cin >> mark_y;

  if (mark_x >= xdim || mark_y >= ydim)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (mark(board, xdim, ydim, mark_x, mark_y) == 2)
  {
    std::cout << "Position already revealed, so cannot be marked." << std::endl;
  }
}

size_t readSizeT()
{
  return 0;
}

size_t xdim_max()
{
  return 60;
}

size_t ydim_max()
{
  return 20;
}

char markedBit()
{
  return 0x10; // 0001 0000
}

char hiddenBit()
{
  return 0x20; // 0010 0000
}

char valueMask()
{
  return 0x0F;
}

void spreadMine(char * board, size_t xdim, size_t ydim, unsigned int numMine)
{
  if (board != NULL)
  {
    for (unsigned int gen_goose {0}; gen_goose < numMine; gen_goose++)
    {
      size_t try_position {0};
      do {
         try_position = rand() % (xdim * ydim);
      } while (board[try_position] != 0);

      board[try_position] = 9;
    }
  }
}
