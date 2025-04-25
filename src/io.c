#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "card.h"

bool loadDeckFromFile(const char*filename){
    FILE*file = fopen(filename,"r");
    if(!file)return false;// if load successful: true
    // read file
    char line[10]; // read line by line from file
    Card*last=NULL; // until the last card
    int count =0;// count the number of cards read

    while(fgets(line, sizeof(line),file)){
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
void saveList(FILE*file, Card*head){
    while(head){
        fprintf(file,"%c%c %d\n",head->rank, head->suit,head->faceUp?1:0);
        head=head->next;
    }
}
void saveGameToFile(const char* filename){

    FILE*file = fopen(filename,"w");
    if(!file)return;
// Save deck
    fprintf(file,"DECK\n");
    saveList(file,deck);
// Save columns
for (int i = 0; i < NUM_COLUMNS; i++) {
    fprintf(file, "COLUMN %d\n", i);
    saveList(file, columns[i]);
}

// Save foundations
for (int i = 0; i < NUM_FOUNDATIONS; i++) {
    fprintf(file, "FOUNDATION %d\n", i);
    saveList(file, foundations[i]);
}
fclose(file);
    printf("Game saved to %s\n", filename);

};
void printBoard();

void printMessage( const char* msg){
    printf("Message: %s\n",msg);
};
