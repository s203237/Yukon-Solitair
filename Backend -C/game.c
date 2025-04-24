#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "card.h"
#include "card.c"

Card* columns[NUM_COLUMNS];
Card* foundations[NUM_FOUNDATIONS];
Card* deck =NULL;

void initGame(){
    for(int i=0; i<NUM_COLUMNS;i++) {
        columns[i]=NULL;
    }
    for(int i=0; i<NUM_FOUNDATIONS;i++) {
        foundations[i]=NULL;
    }
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
int my_random(int n) {
    int seed = (n * 73 + 17) % 1007; //simple but sufficient for basic shuffling
    return seed;
}
void shuffleDeckRandom() {
    //Simple seed-based pseudo-random generator
    int seed = my_random(47);

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
int insert_pos = (length == 0) ? 0 : seed % (length + 1);

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
  while (current != NULL && count < split) {
    prev = current;
    current = current -> next;
    count++;
  }

  if (prev != NULL) {
    prev -> next = NULL;
    }
    secondHalf = current;

    //Interleave cards from both hslves
    Card* shuffled = NULL;
    Card** tail = &shuffled;

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


void dealCards(){
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
void moveCard(int fromCol, char rank, char suit, int toCol ) {
    if(fromCol<0|| fromCol>= NUM_COLUMNS ||toCol>=NUM_COLUMNS) {
        return;
    }
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
        if(!canMoveOnTop(dest,src)) {
            printf("Invalid move. \n");
            return;
        }
        dest->next = src;
    }


}

void printBoard(){
  //Printer column headers
  for(int i = 0; i < NUM_COLUMNS; i++){
    printf("C%d\t", i + 1);
  }
  printf("\tF1\tF2\tF3\tF4\n");
  //Find the tallest column to determine how many rows to print
  int maxRows = 0;
  for(int i = 0; i < NUM_FOUNDATIONS; i++){
    int count = 0;
    Card* temp = columns[i];
    while(temp != NULL){
      count++;
      temp = temp -> next;
    }
    if(count > maxRows) maxRows = count;
  }
  //Print each row of the columns
  for(int row = 0; row < maxRows; row++) {
      for(int col = 0; col < NUM_COLUMNS; col++) {
          Card* temp = columns[col];
          int r = 0;
          while(temp != NULL && r < row) {
              temp = temp -> next;
              r++;
          }
          if(temp != NULL) {
              if(temp->faceUp){
                  printf("%c%c\t", temp->rank, temp->suit);
              }
              else {
                  printf("[]\t");
              }
          } else {
              printf("\t");
          }
          //Print foundations only on the first 4 rows (optional visual alignment)
          if(row == 0) {
              for(int f = 0; f < NUM_FOUNDATIONS; f++){
                  if(foundations[f] != NULL && foundations[f] -> faceUp){
                      printf("%c%c\t", foundations[f] -> rank, foundations[f] -> suit);}
                  else
                  { printf("[]\t");
                  }
              }
              printf("\n");
          }
      }
  }
}
