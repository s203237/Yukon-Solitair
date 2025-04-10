#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "card.h"

Card*columns[NUM_COLUMNS];
Card*foundations[NUM_FOUNDATIONS];
Card*deck =NULL;

void initGame(){
    for(int i=0; i<NUM_COLUMNS;i++)
    columns[i]=NULL;
    for(int i=0; i<NUM_FOUNDATIONS;i++)
    foundations[i]=NULL;
    deck =NULL;

}
// load deck from file
bool loadDeckFromFile(const char*filename){
    FILE*file = fopen(filename,"r");
    if(!file)return false;// if load successful: true
    // read file
    char line[10]; // read line by line from file
    Card*last=NULL; // until the last card
    int count =0;// count the number of cards read

    while(fget(line, sixeof(line),file)){
        if(strlen(line)<2) continue;// skip if line is too short (invalid)
        char rank = line[0];
        char suit = line[1];
        Card*newCard = createCard(rank, suit, false);// create a new card (default is faceUp = false)
        if (!deck) deck = newCard;// add card to deck list
        else last ->next = newCard;
        last = newCard;
        count++;
    }
    fclose(file);
    return count ==52;
}

void shuffleDeckRandom(){

}
void showDeck(){

}
void shuffleDeckSplit(int split){

}

void DealCards(){

}
void moveCard(){

}
void printBoard(){

}