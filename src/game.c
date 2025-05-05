#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "card.h"
#include "io.h"
#include "utils.h" 


Card* columns[NUM_COLUMNS];
Card* foundations[NUM_FOUNDATIONS];
Card* deck =NULL;


void initGame(){
    for(int i=0; i<NUM_COLUMNS;i++) {
        columns[i]=NULL;
    }
    for(int i=0; i<NUM_FOUNDATIONS;i++) {
       // foundations[0]=createCard('A','s',true); //TEST
       foundations[i]=NULL;
    }
    deck =NULL;

}


int my_random(int n) {
    int seed = (n * 73 + 17) % 1007; //simple but sufficient for basic shuffling
    return seed;
}

void shuffleDeckRandom(){
// Convert deck to array for easy shuffling
if (!deck) return;
Card* array[52];
int n=0;
Card*current = deck;

while(current != NULL && n<52){
array[n++]=current;
current =current -> next;

// shuffle
srand(time(NULL));
for(int i=n-1; i>0;i--){
    int j = rand()%(i+1);
    Card*tmp =array[i];
    array[i]=array[j];
    array[j]=tmp;
}
// covert back to linked list
for(int i=0;i<n-1;i++){
    array[i]->next= array[i+1];
    array[i]->faceUp =false;
}
array[n - 1]->next = NULL;
array[n - 1]->faceUp = false;

deck = array[0];

}
}
void shuffleDeckSplit(int split){
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
// This method will flip all cards in the columns face up.
void showAllCards() {
    for (int i = 0; i < NUM_COLUMNS; i++) {
        Card* current = columns[i];
        while (current != NULL) {
            current->faceUp = true;
            current = current->next;
        }
    }
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
        int total = (col < 3) ? 8 : 7;  // C1–C3: 8 cards, C4–C7: 7 cards
    
        Card*prev =NULL;

        for(int i =0;i<total;i++){
            if(!current) return;
            Card*next = current->next;
            current->next =NULL;
            current->faceUp =false;

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
        printf("Invalid column index.\n");
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
    if (!columns[toCol]) {
        if (rank != 'K') {
            printf("Only King can be moved to empty column.\n");
            if (prev) prev->next = src;
            else columns[fromCol] = src;
            return;
        }
        columns[toCol] = src;
    }else {
        Card*dest = columns[toCol];
        while(dest->next) dest =dest->next;
        // check: can it be connected?
        if (!isValidMove(src, dest)) {
            printf("Invalid move: doesn't follow Yukon rules.\n");
            if (prev) prev->next = src;
            else columns[fromCol] = src;
            return;
        }
        dest->next = src;
    }


}

void printBoard() {
    //  1. Prints columns title C1 - C7.
    for (int i = 0; i < NUM_COLUMNS; i++) {
        printf("C%d\t", i + 1);
    }
    printf("\n\n");

    //  2. Find maxim number of cards in columns.
    int maxRows = 0;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        int count = 0;
        Card* temp = columns[i];
        while (temp != NULL) {
            count++;
            temp = temp->next;
        }
        if (count > maxRows) maxRows = count;
    }

    // Make sure that there 4 rows to show F1-F4.
    if (maxRows < NUM_FOUNDATIONS) maxRows = NUM_FOUNDATIONS;

    //  Prints each row of the cards
    for (int row = 0; row < maxRows; row++) {
        for (int col = 0; col < NUM_COLUMNS; col++) {
            Card* temp = columns[col];
            int r = 0;

            // Move down the linked list to the card at the current row
            while (temp != NULL && r < row) {
                temp = temp->next;
                r++;
            }
    
            // Prints the card or an empty space
            if (temp != NULL) {
                if (temp->faceUp) {
                    printf("%c%c\t", temp->rank, temp->suit);
                } else {
                    printf("[]\t");
                }
            } else {
                printf("\t");
            }
            
        }

        // Prints foundation name F1-F4 to the right side.
        if (row < NUM_FOUNDATIONS) {
            if (foundations[row] != NULL) {
                if (foundations[row]->faceUp)
                    printf("\t[%c%c]", foundations[row]->rank, foundations[row]->suit);
                else
                    printf("\t[]");
            } else {
                printf("\t[]");
            }
        
            // print name F1 - F4
            printf("\tF%d", row + 1);
            printf("\n\n");
        } else{
   
            printf("\n");}
        
    }
}
    char* getBoardString() {
        static char buffer[2048];
        buffer[0] = '\0';  //

        // Columns
        for (int col = 0; col < NUM_COLUMNS; col++) {
            Card* card = columns[col];
            while (card) {
                if (card->faceUp) {
                    char cardStr[4];
                    snprintf(cardStr, sizeof(cardStr), "%c%c ", card->rank, card->suit);
                    strcat(buffer, cardStr);
                } else {
                    strcat(buffer, "[] ");
                }
                card = card->next;
            }
            strcat(buffer, "\n");
        }

        // Foundations
        strcat(buffer, "---\n");
        for (int f = 0; f < NUM_FOUNDATIONS; f++) {
            if (foundations[f] && foundations[f]->faceUp) {
                char fStr[4];
                snprintf(fStr, sizeof(fStr), "%c%c ", foundations[f]->rank, foundations[f]->suit);
                strcat(buffer, fStr);
            } else {
                strcat(buffer, "__ ");
            }
        }
        strcat(buffer, "\n");

        return buffer;
    }


