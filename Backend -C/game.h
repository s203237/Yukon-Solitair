#ifndef GAME_H
#define GAME_H

#include "card.h"


#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4

extern Card* columns[NUM_COLUMNS];
extern Card* foundations[NUM_FOUNDATIONS];
extern Card* deck;

void initGame();
bool loadDeckFromFile(const char* filename);
void showDeck();
void DealCards();
void shuffleDeckRandom();
void shuffleDeckSplit(int split);
void moveCard();
void printBoard();


#endif

