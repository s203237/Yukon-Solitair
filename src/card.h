#ifndef CARD_H
#define CARD_H
#include <stdbool.h>

typedef struct Card{
    char rank; //'A','2'...'K'
    char suit; //'C', 'D', 'H','S'
    bool faceUp;// true if card is open, false if card is close
    struct Card* next;
    struct Card* prev;
}Card;
// create new card
Card* createCard(char rank, char suit, bool faceUp);
//

void printCard(const Card* card);
#endif
