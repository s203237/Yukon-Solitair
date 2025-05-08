#ifndef GAME_H
#define GAME_H
#include "card.h"
#include "board.h"



#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4

extern Card* columns[NUM_COLUMNS];
extern Card* foundations[NUM_FOUNDATIONS];
extern Card* deck;
Board* initBoard();
void showAllCards(Board*board);
void shuffleDeckRandom(Card* deck);
void shuffleDeckSplit(Card* deck, int split);
void moveCard(int fromCol, char rank, char suit, int toCol);
void dealCardsForPlayPhase(Board* board, Card* deck);
void dealDeckForGridView(Board* board, Card* deck);



#endif

