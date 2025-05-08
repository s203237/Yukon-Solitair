#ifndef IO_H
#define IO_H
#include "card.h"
#include <stdio.h>
#include "board.h"


Card* loadDeckFromFile(const char* filename);
void printMessage( const char* msg);
void saveList(FILE*file, Card*head);
void saveGameToFile(Board* board, Card* deck, const char* filename);
void printBoard(Board* board);
char* getBoardString(Board* board);

#endif