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
    Card*current = deck;

    for (int col=0; col<NUM_COLUMNS; col++){
        int faceDown = col;
        int faceUp =5; //there are always 5 open cards/column (c2-c7)
        int total = faceDown+faceUp;

        Card*prev =NULL;

        for(int i =0;i<total;i++){
            if(!current) return;
            Card*next = current->next;
            current->next =NULL;
            current->faceUp =(i>=faceDown);

            if(!columns[col]){
                columns[col]=current;
            } else {
                prev ->next=current;
            }
            prev = current;
            current = next;
        }
    }
deck =current;
}
// to move a card from fromCol to toCol
void moveCard(int fromCol, char rank, char suit, int toCol ){
    if(fromCol<0|| fromCol>= NUM_COLUMNS ||toCol>=NUM_COLUMNS)return;
    Card* src = columns[fromCol];
    Card*prev =NULL;
    // find a card to move
    while(src&& (src-> rank !=rank|| src -> suit!=suit|| !src->faceUp)){
    prev =src;
    src =src->next;
    }
    if(!src)return; //Not found 
    // if there is  prev -> stop at src
    if(prev){
        prev ->next =NULL;
    }else {
        columns[fromCol] =NULL;
    }
    //find the tail
    Card*tail =src;
    while(tail->next)tail =tail->next;
    // add toCol
    if(!columns[toCol]){
        columns[toCol]=src;
    }else {
        Card*dest = columns[toCol];
        while(dest->next) dest =dest->next;
        // check: can it be connected?
        if(!canMoveOnTop(dest,src)){
            printf("Invalid move. \n");
            return;
        }
        dest->next = src;
    }

}


void printBoard(){

}