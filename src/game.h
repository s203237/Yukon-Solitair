#ifndef GAME_H
#define GAME_H

#include "card.h"


#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4

extern Card* columns[NUM_COLUMNS];
extern Card* foundations[NUM_FOUNDATIONS];
extern Card* deck;

void initGame();
void showDeck();
void dealCards();
void shuffleDeckRandom();
void shuffleDeckSplit(int split);
void moveCard(int fromCol, char rank, char suit, int toCol);
void printBoard();


#endif

