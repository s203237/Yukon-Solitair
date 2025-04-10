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
//Simple seed-based pseudo-random generator
int seed = 47;

int my_random() {
seed = (seed * 73 + 17) % 1007; //simple but sufficient for basic shuffling
return seed;
}

if (!deck) return;

Card* newDeck = NULL; // the shuffled deck
Card* current = deck;
deck = NULL;  // empty the original deck

while(current != NULL){
Card* next = current->next; // store the next card
current -> next = NULL;

// count how many cards are currently in newDeck
int length = 0;
Card* temp = newDeck;
while(temp != NULL){
length++;
temp = temp->next;
}
//Choose a random insertion position in newDeck
int insert_pos = (length == 0) ? 0 : my_random() % (length + 1);

if( insert_pos == 0) {
current -> next = newDeck;
newDeck = current;
} else {
//Insert in the middle or at the end
Card* prev = newDeck;
for(int i = 1; i < insert_pos; i++) {
prev = prev->next;
}
current -> next = prev -> next;
prev -> next = current;
}
current = next;
}
deck = newDeck;

}
void suffleDeckSplit(int split){
  if(!deck || split == 0) return;

  //Split the deck into two parts
  Card* firstHalf = deck;
  Card* secondHalf = NULL;
  Card* current = deck;
  Card* prev = NULL;

  int count = 0;
  while (current !NULL && count < split) {
    prev = current;
    current = current -> next;
    count++;
  }

  if (prev !NULL) {
    prev -> next = NULL;
    }
    secondHalf = current;

    //Interleave cards from both hslves
    Card* shuffled = NULL;
    Card** tail = &sheffled;

    while(firstHalf != NULL || secondHalf != NULL){
      if(firstHalf != NULL){
        *tail = firstHalf;
        tail = &firstHalf->next;
        firstHalf = *tail;
      }
      if(secondHalf != NULL){
        *tail = secondHalf;
        tail = &secondHalf->next;
        secondHalf = *tail;
      }
    }
    deck = shuffled;


}
void showDeck(){
  Card* current = deck;
  while(current != NULL){
    if(current -> faceUp) {
      printf("%c%c", current -> rank, current -> suit); // prints faceUp cards
      } else {
        printf("[] "); // prints faceDown cards
  }
  current = current -> next;
  }
  printf("\n");

}


void DealCards(){

}
void moveCard(){

}
void printBoard(){

}