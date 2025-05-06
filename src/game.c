#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
#include "card.h"
#include "io.h"
#include "utils.h" 

typedef struct column{
    Card* head;
    Card* tail;
    struct  column*next;
    struct column*prev;
    }Column;
typedef struct foundation{
    Card*head;
    Card*tail;
    struct foundation*next;
    struct foundation*prev;
}Foundation;
typedef struct head{
    Card*head;
    Card*tail;
    Foundation*foundationHead;
    Foundation*foundationTail;
}Board;

    

// void initGame(){
//     for(int i=0; i<NUM_COLUMNS;i++) {
//         columns[i]=NULL;
//     }
//     for(int i=0; i<NUM_FOUNDATIONS;i++) {
//        // foundations[0]=createCard('A','s',true); //TEST
//        foundations[i]=NULL;
//     }
//     deck =NULL;

// }
Board* initBoard() {
    Board* gameBoard = malloc(sizeof(Board));

    gameBoard->head = malloc(sizeof(Column));
    gameBoard->tail = malloc(sizeof(Column));
    gameBoard->head->next = gameBoard->tail;
    gameBoard->head->prev = NULL;
    gameBoard->tail->next = NULL;
    gameBoard->tail->prev = gameBoard->head;

    gameBoard->foundationHead = malloc(sizeof(Foundation));
    gameBoard->foundationTail = malloc(sizeof(Foundation));
    gameBoard->foundationHead->next = gameBoard->foundationTail;
    gameBoard->foundationHead->prev = NULL;
    gameBoard->foundationTail->next = NULL;
    gameBoard->foundationTail->prev = gameBoard->foundationHead;

    // Initialize 7 columns
    Column* curr = gameBoard->head;
    for (int i = 0; i < 7; i++) {
        Column* col = malloc(sizeof(Column));
        col->head = malloc(sizeof(Card));
        col->tail = malloc(sizeof(Card));
        col->head->next = col->tail;
        col->head->prev = NULL;
        col->tail->prev = col->head;
        col->tail->next = NULL;
        col->next = curr->next;
        col->prev = curr;
        curr->next->prev = col;
        curr->next = col;
        curr = col;
    }

    // Initialize 4 foundation
    Foundation* fond = gameBoard->foundationHead;
    for (int j = 0; j < 4; j++) {
        Foundation* f = malloc(sizeof(Foundation));
        f->head = malloc(sizeof(Card));
        f->tail = malloc(sizeof(Card));
        f->head->next = f->tail;
        f->head->prev = NULL;
        f->tail->prev = f->head;
        f->tail->next = NULL;
        f->next = fond->next;
        f->prev = fond;
        fond->next->prev = f;
        fond->next = f;
        fond = f;
    }

    return gameBoard;
}

// This method will flip all cards in the columns face up.
void showAllCards(Board*board) {
    for(Column*col=board->head->next; col!= board-> tail; col=col->next){
        for(Card*c=col->head->next; c!= col->tail; c=c->next){
            c->faceUp=true;
        }
    }
}
// void showDeck(){
//     Card* current = deck;
//     while(current != NULL){
//       if(current -> faceUp) {
//         printf("%c%c", current -> rank, current -> suit); // prints faceUp cards
//         } else {
//           printf("[] "); // prints faceDown cards
//     }
//     current = current -> next;
//     }
//     printf("\n");
  
//   }


void dealCardsForPlayPhase(Board* board, Card* deck) {
    Column* col = board->head->next;
    Card* current = deck;

    for (int colIndex = 0; colIndex < 7 && col != board->tail; colIndex++, col = col->next) {
        int total = colIndex + 1 + (colIndex >= 1 ? 5 : 0); // C1=1, C2=6, ..., C7=11
        Card* prev = col->head;

        for (int i = 0; i < total; i++) {
            if (!current) return;

            Card* next = current->next;
            current->next = prev->next;
            prev->next->prev = current;
            prev->next = current;
            current->prev = prev;

            // Only the last card is turned up
            current->faceUp = (i == total - 1);

            prev = current;
            current = next;
        }
    }
}


void dealDeckForGridView(Board* board, Card* deck) {
    Column* col = board->head->next;
    Card* current = deck;
    int colIndex = 0;

    while (col != board->tail && colIndex < 7) {
        int total = (colIndex < 3) ? 8 : 7;
        Card* prev = col->head;

        for (int i = 0; i < total; i++) {
            if (!current) return;

            Card* next = current->next;
            current->next = prev->next;
            prev->next->prev = current;
            prev->next = current;
            current->prev = prev;
        
            current->faceUp = true;
            prev = current;
            current = next;
        }

        col = col->next;
        colIndex++;
    }
}


void shuffleDeckSplit(Card** deck, int split){
  if(!deck ||!(*deck)|| split <= 0) return;

  //Split the deck into two parts
  Card* firstHalf = deck;
  Card* secondHalf = NULL;
  Card* current = *deck;
  Card* prev = NULL;

  int count = 0;
  while (current && count < split) {
    prev = current;
    current = current -> next;
    count++;
  }

  if (prev) {
    prev->next = NULL;
    if (current) current->prev = NULL;
    secondHalf = current;
}
    secondHalf = current;

    //Interleave cards from both hslves
    Card* shuffled = NULL;
    Card** tail = &shuffled;

    while(firstHalf || secondHalf){
      if(firstHalf){
        if (!shuffled) {
            shuffled = firstHalf;
            tail = firstHalf;
        } else {
            tail->next = firstHalf;
            firstHalf->prev = tail;
            tail = firstHalf;
        }
        firstHalf = firstHalf->next;
    }

    if (secondHalf) {
        if (!shuffled) {
            shuffled = secondHalf;
            tail = secondHalf;
        } else {
            tail->next = secondHalf;
            secondHalf->prev = tail;
            tail = secondHalf;
        }
        secondHalf = secondHalf->next;
    }
}

if (tail) tail->next = NULL;
    deck = shuffled;
}



// to move a card from fromCol to toCol
// void moveCard(int fromCol, char rank, char suit, int toCol ) {
//     if(fromCol<0|| fromCol>= NUM_COLUMNS ||toCol>=NUM_COLUMNS) {
//         printf("Invalid column index.\n");
//         return;
//     }
//     Card* src = columns[fromCol];
//     Card*prev =NULL;
//     // find a card to move
//     while(src&& (src-> rank !=rank|| src -> suit!=suit|| !src->faceUp)){
//     prev =src;
//     src =src->next;
//     }
//     if(!src)return; //Not found 
//     // if there is  prev -> stop at src
//     if(prev){
//         prev ->next =NULL;
//     }else {
//         columns[fromCol] =NULL;
//     }
//     //find the tail
//     Card*tail =src;
//     while(tail->next)tail =tail->next;
//     // add toCol
//     if (!columns[toCol]) {
//         if (rank != 'K') {
//             printf("Only King can be moved to empty column.\n");
//             if (prev) prev->next = src;
//             else columns[fromCol] = src;
//             return;
//         }
//         columns[toCol] = src;
//     }else {
//         Card*dest = columns[toCol];
//         while(dest->next) dest =dest->next;
//         // check: can it be connected?
//         if (!isValidMove(src, dest)) {
//             printf("Invalid move: doesn't follow Yukon rules.\n");
//             if (prev) prev->next = src;
//             else columns[fromCol] = src;
//             return;
//         }
//         dest->next = src;
//     }


// }

void printBoard(Board* board) {
    // Prints columns title C1 - C7.
    for (int i = 1; i <= 7; i++) printf("C%d\t", i);
    printf("\n\n");

    // Find maxim number of cards in columns
    int maxRows = 0;
    for (Column* col = board->head->next; col != board->tail; col = col->next) {
        int count = 0;
        for (Card* c = col->head->next; c != col->tail; c = c->next) count++;
        if (count > maxRows) maxRows = count;
    }

    // Print each line
    for (int row = 0; row < maxRows; row++) {
        Column* col = board->head->next;
        while (col != board->tail) {
            Card* c = col->head->next;
            for (int i = 0; i < row && c != col->tail; i++) c = c->next;
            if (c != col->tail)
                printf(c->faceUp ? "%c%c\t" : "[]\t", c->rank, c->suit);
            else
                printf("\t");
            col = col->next;
        }
        printf("\n");
    }

    // In Foundation
    printf("\n");
    Foundation* f = board->foundationHead->next;
    for (int i = 1; f != board->foundationTail; f = f->next, i++) {
        Card* top = f->tail->prev;
        if (top != f->head) {
            printf("[%c%c] F%d\t", top->rank, top->suit, i);
        } else {
            printf("[] F%d\t", i);
        }

    }
    printf("\n");
}



