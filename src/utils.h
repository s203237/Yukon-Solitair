#ifndef UTILS_H
#define UTILS_H
#include "card.h"


int rankToValue(char rank);
bool isRed(char suit);
bool isValidMove(Card* fromCard, Card* toCard );

#endif
