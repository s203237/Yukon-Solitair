#ifndef BOARD_H
#define BOARD_H

#include "card.h"

typedef struct column {
    Card* head;
    Card* tail;
    struct column* next;
    struct column* prev;
} Column;

typedef struct foundation {
    Card* head;
    Card* tail;
    struct foundation* next;
    struct foundation* prev;
} Foundation;

typedef struct board {
    Column* head;
    Column* tail;
    Foundation* foundationHead;
    Foundation* foundationTail;
} Board;

#endif
