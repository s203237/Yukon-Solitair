#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "card.h"
#include "board.h"

Card* loadDeckFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    char line[10];
    Card* head = NULL;
    Card* last = NULL;
    int count = 0;

    while(fgets(line, sizeof(line),file)){
        if(strlen(line)<2) continue;// skip if line is too short (invalid)
        char rank = line[0];
        char suit = line[1];
        Card*newCard = createCard(rank, suit, false);// create a new card (default is faceUp = false)
        if (!head) head = newCard;
        else last->next = newCard;
        last = newCard;
        count++;
    }
    fclose(file);
    return (count == 52) ? head : NULL;
}


void saveList(FILE*file, Card*head){
    while(head){
        fprintf(file,"%c%c %d\n",head->rank, head->suit,head->faceUp?1:0);
        head=head->next;
    }
}
void saveGameToFile(Board* board, Card* deck, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    fprintf(file, "DECK\n");
    saveList(file, deck);

    int colIndex = 0;
    for (Column* col = board->head->next; col != board->tail; col = col->next, colIndex++) {
        fprintf(file, "COLUMN %d\n", colIndex);
        saveList(file, col->head->next);
    }

    int fIndex = 0;
    for (Foundation* f = board->foundationHead->next; f != board->foundationTail; f = f->next, fIndex++) {
        fprintf(file, "FOUNDATION %d\n", fIndex);
        saveList(file, f->head->next);
    }

    fclose(file);
    printf("Game saved to %s\n", filename);
}


void printMessage( const char* msg){
    printf("Message: %s\n",msg);
};

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

    }git add .
git commit -m "Finish previous merge"

    printf("\n");
}
char* getBoardString(Board* board) {
        static char buffer[2048];
        buffer[0] = '\0';  //

        // Columns
        int colIndex = 1;
    for (Column* col = board->head->next; col != board->tail; col = col->next, colIndex++) {
        strcat(buffer, "C");
        char num[4];
        sprintf(num, "%d", colIndex);
        strcat(buffer, num);
        strcat(buffer, ": ");

        for (Card* card = col->head->next; card != col->tail; card = card->next) {
            if (card->faceUp) {
                char cardStr[4];
                snprintf(cardStr, sizeof(cardStr), "%c%c ", card->rank, card->suit);
                strcat(buffer, cardStr);
            } else {
                strcat(buffer, "[] ");
            }
        }
        strcat(buffer, "\n");
    }

    // Foundations
    strcat(buffer, "---\n");
    int fIndex = 1;
    for (Foundation* f = board->foundationHead->next; f != board->foundationTail; f = f->next, fIndex++) {
        strcat(buffer, "F");
        char fnum[4];
        sprintf(fnum, "%d", fIndex);
        strcat(buffer, fnum);
        strcat(buffer, ": ");

        Card* top = f->tail->prev;
        if (top != f->head && top != NULL) {
            char fStr[4];
            snprintf(fStr, sizeof(fStr), "%c%c ", top->rank, top->suit);
            strcat(buffer, fStr);
        } else {
            strcat(buffer, "__ ");
        }
        strcat(buffer, "\n");
    }

    return buffer;
    }