#ifndef IO_H
#define IO_H
#include "card.h"
#include <stdio.h>


bool loadDeckFromFile(const char* filename);
void printBoard();
void printMessage( const char* msg);
void saveList(FILE*file, Card*head);
void saveGameToFile(const char* filename);
#endif