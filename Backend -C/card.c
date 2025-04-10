#include<stdio.h>
#include<stdlib.h>
#include"card.h"

//create new card
Card* createCard(char rank, char suit, int faceUp){
    Card*newCard = (Card*)malloc(sizeof(Card));
    if(!newCard)return NULL;
    newCard ->rank = rank;
    newCard ->suit =suit;
    newCard -> faceUp =faceUp;
    newCard -> next =NULL;
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