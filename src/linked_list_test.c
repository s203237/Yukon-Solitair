#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "card.c"
#include <unistd.h>
#include <string.h>

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

void loadFromFile(const char* path, const Board* board) {
    bool seen[52] = {false};

    FILE* file = NULL;
    if (access(path, F_OK) == 0) {
        if (access(path, R_OK) == 0) {
            file = fopen(path, "r");
        } else {
            printf("Could not read from this file");
            return;
        }
    } else {
        printf("File doesn't exist");
        return;
    }
    if (file) {
        int col = 1, row = 1;
        for (int i = 1; i <= 53; i++) {
            char line[4];
            const char* ret = fgets(line, 4, file);
            if (ret != NULL) {
                if (i <= 52) {
                    line[strcspn(line, "\n")] = '\0';
                    if (strlen(line) == 2) {
                        const char* suits = "CDHS";
                        const char* ranks = "A23456789TJQK";
                        char rank = line[0], suit = line[1];
                        char* rp = strchr(ranks, rank);
                        char* sp = strchr(suits, suit);
                        if (!rp || !sp) {
                            printf("There is an illegal card %s on line %i", line, i);
                            emptyBoard(board);
                            return;
                        }
                        int rankidx = (int)(rp - ranks);
                        int suitidx = (int)(sp - suits);
                        int idx = suitidx * 13 + rankidx;

                        if (seen[idx]) {
                            printf("Duplicate card at line %i", i);
                            emptyBoard(board);
                            return;
                        } else {
                            seen[idx] = true;
                            Card* card = createCard(rank, suit, false);
                            addCard(board, card, col, row);
                            printf("The card %c of %c has been added at col %i, row %i\n", card->rank, card->suit, col, row);
                            col++;
                            if (col > 7) {
                                col = 1;
                                row++;
                            }
                        }
                    } else {
                        printf("line %i contains an invalid card %s that is either too long or short\n", i, line);
                        emptyBoard(board);
                        return;
                    }
                    //Validate and add
                    //if invalid empty board that is being filled and error
                } else {
                    printf("Too many lines in this file");
                    emptyBoard(board);
                    return;
                }
            } else if (i < 52) {
                printf("There is only %i lines in this file", i-1);
                emptyBoard(board);
                return;
            }

        }
    } else {
        printf("Something went wrong while loading the file");
    }
}

/*void loadCommand(FILE* file, Board* board) {
    if (boardHasCard(board)) {
        emptyBoard(board);
        loadFromFile(file, board);
    } else {
        loadFromFile(file, board);
    }
}*/



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
