#include <stdio.h>
#include <stdlib.h>
#include "card.h"

typedef struct column {
    Card* head;
    Card* tail;
    struct column* next;
    struct column* prev;
}Column;
typedef struct foundation {
    Card* head;
    Card* tail;
    struct foundation* next;
    struct foundation* prev;
}Foundation;

typedef struct header {
    Column* head;
    Column* tail;
    Foundation* foundationHead;
    Foundation* foundationTail;
}Board;


Board* initBoard() {
    Board* gameBoard = malloc(sizeof(Board));
    gameBoard->head = (Column*) malloc(sizeof(Column));
    gameBoard->tail = (Column*) malloc(sizeof(Column));
    gameBoard->head->next = gameBoard->tail;
    gameBoard->head->prev = NULL;
    gameBoard->tail->next = NULL;
    gameBoard->tail->prev = gameBoard->head;
    gameBoard->foundationHead = (Foundation*) malloc(sizeof(Foundation));
    gameBoard->foundationTail = (Foundation*) malloc(sizeof(Foundation));
    gameBoard->foundationHead->next = gameBoard->foundationTail;
    gameBoard->foundationHead->prev = NULL;
    gameBoard->foundationTail->next = NULL;
    gameBoard->foundationTail->prev = gameBoard->foundationHead;
    Column* curr_col = gameBoard->head;
    Foundation* curr_f = gameBoard->foundationHead;
    for (int i = 0; i < 7; i++) {
        Column* newColumn = malloc(sizeof(Column));
        newColumn->head = (Card*) malloc(sizeof(Card));
        newColumn->tail = (Card*) malloc(sizeof(Card));
        newColumn->head->next = newColumn->tail;
        newColumn->head->prev = NULL;
        newColumn->tail->next = NULL;
        newColumn->tail->prev = newColumn->head;
        newColumn->next = curr_col->next;
        newColumn->prev = curr_col;
        curr_col->next->prev = newColumn;
        curr_col->next = newColumn;
        curr_col = newColumn;
    }
    for (int j = 0; j < 4; j++) {
        Foundation* newFoundation = malloc(sizeof(Foundation));
        newFoundation->head = (Card*) malloc(sizeof(Card));
        newFoundation->tail = (Card*) malloc(sizeof(Card));
        newFoundation->head->next = newFoundation->tail;
        newFoundation->head->prev = NULL;
        newFoundation->tail->next = NULL;
        newFoundation->tail->prev = newFoundation->head;
        newFoundation->next = curr_f->next;
        newFoundation->prev = curr_f;
        curr_f->next->prev = newFoundation;
        curr_f->next = newFoundation;
        curr_f = newFoundation;

    }
    return gameBoard;
}

int addCard(const Board* board, Card* card, const int col, const int row) {
    Card* newCard = card;
    const Column* curr_col = board->head;
    if (row <= 0 || row > 13 || col <= 0 || col > 7) {
        printf("Col %d, row %d is outside of the playable board\n", col, row);
        return 0;
    }
    for (int i = 0; i < col; i++) {
        if (curr_col->next == board->tail) {
            if (i == col - 1) {
                break;
            } else {
                printf("col out of bounds");
                return 0;
            }
        }
        curr_col = curr_col->next;
    }
    Card* curr_node = curr_col->head;
    for (int j = 0; j < row; j++) {
        if (curr_node->next == curr_col->tail) {
            if (j == row - 1) {
                break;
            } else {
                printf("_____________________\nThere are not enough cards in col %d to add a card at row %d.\n_____________________\n", col, row);
                return 0;
            }
        }
        curr_node = curr_node->next;
    }
    newCard->next = curr_node->next;
    newCard->prev = curr_node;
    curr_node->next->prev = newCard;
    curr_node->next = newCard;
    return 1;
}

bool boardHasCard(const Board* board) {
    for (const Column* col = board->head->next; col != board->tail; col = col->next) {
        if (col->head->next != col->tail) {
            return true;
        }
    }
    for (const Foundation* fond = board->foundationHead->next; fond != board->foundationTail; fond = fond->next) {
        if (fond->head->next != fond->tail) {
            return true;
        }
    }
    return false;
}

void emptyBoard(const Board* board) {
    for (const Column* col = board->head->next; col != board->tail; col = col->next) {
        printf("Emptying column %p (head=%p, tail=%p)\n", (void*)col, (void*)col->head, (void*)col->tail);
        Card* card = col->head->next;
        while(card != col->tail) {
                Card* nextCard = card->next;
                printf("  freeing card %p (->next = %p)\n", (void*)card, (void*)nextCard);
                free(card);
                card = nextCard;
        }
        col->head->next = col->tail;
        col->tail->prev = col->head;
    }
    for (const Foundation* fond = board->foundationHead->next; fond != board->foundationTail; fond = fond->next) {
        Card* card = fond->head->next;
        while (card != fond->tail) {
            Card* nextCard = card->next;
            free(card);
            card = nextCard;
        }
        fond->head->next = fond->tail;
        fond->tail->prev = fond->head;
    }
}

void loadFromFile(FILE* file, Board* board) {
    //Check does file exist
    //if exists then {
        //attempt to load
        //validate line if valid then {
            //AddCard
        //} else {
            //emptyBoard
            //Throw error
        //}
    //} else {
        //Throw error
    //}
}

void loadCommand(FILE* file, Board* board) {
    if (boardHasCard(board)) {
        emptyBoard(board);
        loadFromFile(file, board);
    } else {
        loadFromFile(file, board);
    }
}



void moveCard(Card* card) { //Also pass target location somehow
    //check if legal move
    //unlink card from current position
    //attach to new position
    //else throw error
}

/*Card* locateCard(int col, int row) {
    Card* temp = NULL;
    for (int i = 0; i < col; i++) {
        temp =  Deck
    }
    return card;
}*/
