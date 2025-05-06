#include<stdio.h>
#include<stdlib.h>
#include"card.h"

//create new card
Card* createCard(char rank, char suit, bool faceUp){
    Card*newCard = (Card*)malloc(sizeof(Card));
    if(!newCard)return NULL;
    newCard ->rank = rank;
    newCard ->suit =suit;
    newCard -> faceUp =faceUp;
    newCard -> next =NULL;
    newCard->prev =NULL;
    return newCard;
}
//free the entire linked list
void freeCardList(Card*head){
    while(head){
        Card*temp =head;
        head =head->next;
        free(temp);
    }
}
// print card in terminal
void printCard(const Card*card){
    if(card ==NULL){
        printf("\t");
    }else if (card ->faceUp){
        printf("%c%c\t", card->rank, card->suit);
    }else {
        printf("[]\t"); // card is close
    }
}
// //Compare to see if this card can be placed on another card
// bool canMoveOnTop(const Card*top, const Card*bottom){
//     if(!top||!bottom) return false;
// // check cards color: 
//     bool isRedTop = (top->suit == 'H' || top->suit == 'D');
//     bool isRedBottom = (bottom->suit == 'H' || bottom->suit == 'D');
//     if (isRedTop == isRedBottom) return false;
// compare the order of cards
// char ranks[] = "A23456789TJQK";
  
// int indexTop = -1, indexBottom = -1;
// for (int i = 0; i < 13; i++) {
//     if (ranks[i] == top->rank) indexTop = i;
//     if (ranks[i] == bottom->rank) indexBottom = i;
// }
// return indexTop + 1 == indexBottom;

// }