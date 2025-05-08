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
#include "board.h"


Card* globalDeck = NULL;


  
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


void shuffleDeckSplit(Card* deck, int split){
  if(!deck || split <= 0) return;

  //Split 
  Card* current = deck;
  Card* prev = NULL;

  int count = 0;
  while (current && count < split) {
    prev = current;
    current = current -> next;
    count++;
  }

  if (!prev) return;
    prev->next = NULL;
    Card* firstHalf = deck;
    Card* secondHalf = current;

    //Interleave cards from both hslves
    Card* shuffled = NULL;
    Card** tail = &shuffled;

    while (firstHalf || secondHalf) {
        if (firstHalf) {
            *tail = firstHalf;
            firstHalf = firstHalf->next;
            tail = &((*tail)->next);
        }
        if (secondHalf) {
            *tail = secondHalf;
            secondHalf = secondHalf->next;
            tail = &((*tail)->next);
        }
    }
    *tail = NULL;

    extern Card* globalDeck;
    globalDeck = shuffled;
}



    void shuffleDeckRandom(Card* deck) {
        if (!deck ) return;
    
        // Seed random
        srand(time(NULL));
        Card* shuffled = NULL;
        Card* current = deck;
    
        // Loop through each card in the original deck
        while (current) {
            Card* next = current->next;
            current->next = NULL;
    
            // Calculate the length of the current shuffled
            int length = 0;
            for (Card* temp = shuffled; temp; temp = temp->next) length++;
    
            // Select random location
            int pos = (length == 0) ? 0 : rand() % (length + 1);
    
            if (pos == 0) {
                // Insert head
                current->next = shuffled;
                shuffled = current;
            } else {
                // Insert middle or end
                Card* prev = shuffled;
                for (int i = 1; i < pos; i++) prev = prev->next;
                current->next = prev->next;
                prev->next = current;
            }
    
            current = next;
        }
    
        extern Card* globalDeck;
    globalDeck = shuffled; 
    }
    







