#ifndef CARD-H
#define CARD_H

typedef struct Card{
    char rank; //'A','2'...'K'
    char suit; //'C', 'D', 'H','S'
    bool faceUp;// true if card is open, false if card is close
struct Card*next;
}Card;
// create new card
Card*createCard(char rank, char suit, bool faceUp);
//
void freeCardList(Card*head);
void printCard(const Card*head);
#endif
