#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "card.c"
#include <unistd.h>
#include <string.h>
#include <time.h>

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

typedef struct {
    Column* head;
    Column* tail;
    Foundation* foundationHead;
    Foundation* foundationTail;

    char msg[256];
}Board;

typedef struct {
    Card* card;
    int col;
    int row;
} cardLocation;

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
    gameBoard->msg[0] = '\0';
    return gameBoard;
}

void setMessage(const Board* board, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(board->msg, 256, fmt, args);
    va_end(args);
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
        Card* card = col->head->next;
        while(card != col->tail) {
                Card* nextCard = card->next;
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

void createDefaultDeck(const Board* board) {
    int col = 1, row = 1;

    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            const char* suits = "CDHS";
            const char* ranks = "A23456789TJQK";
            Card* card = createCard(ranks[rank], suits[suit], false);
            addCard(board, card, col, row);

            if (++col > 7) {
                col = 1;
                ++row;
            }
        }
    }
}

bool loadFromFile(const char* path, const Board* board) {
    bool seen[52] = {false};

    FILE* file = NULL;
    if (!path) {
        createDefaultDeck(board);
        setMessage(board, "OK");
        return true;
    }
    if (access(path, F_OK) == 0) {
        if (access(path, R_OK) == 0) {
            file = fopen(path, "r");
        } else {
            setMessage(board, "Could not read from this file");
            return false;
        }
    } else {
        setMessage(board, "File does not exist");
        return false;
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
                            setMessage(board,"There is an illegal card %s on line %i", line, i);
                            emptyBoard(board);
                            return false;
                        }
                        int rankidx = (int)(rp - ranks);
                        int suitidx = (int)(sp - suits);
                        int idx = suitidx * 13 + rankidx;

                        if (seen[idx]) {
                            setMessage(board, "Duplicate card at line %i", i);
                            emptyBoard(board);
                            return false;
                        } else {
                            seen[idx] = true;
                            Card* card = createCard(rank, suit, false);
                            addCard(board, card, col, row);
                            col++;
                            if (col > 7) {
                                col = 1;
                                row++;
                            }
                        }
                    } else {
                        setMessage(board, "line %i contains an invalid card \"%s\"", i, line);
                        emptyBoard(board);
                        return false;
                    }
                } else {
                    setMessage(board,"Too many lines in this file");
                    emptyBoard(board);
                    return false;
                }
            } else if (i < 52) {
                setMessage(board,"There are only %i lines in this file", i-1);
                emptyBoard(board);
                return false;
            }

        }
    } else {
        setMessage(board, "Something went wrong while loading the file");
        return false;
    }
    setMessage(board, "OK");
    return true;
}

void moveCard(const Board* board, Card* card, const int targetCol, const int targetRow) {
    card->prev->next = card->next;
    card->next->prev = card->prev;

    const Column* curr_col = board->head;
    if (targetRow <= 0 || targetRow > 13 || targetCol <= 0 || targetCol > 7) {
        printf("Col %d, row %d is outside of the playable board\n", targetCol, targetRow);
        return;
    }
    for (int i = 0; i < targetCol; i++) {
        if (curr_col->next == board->tail) {
            if (i == targetCol - 1) {
                break;
            } else {
                printf("col out of bounds\n");
                return;
            }
        }
        curr_col = curr_col->next;
    }

    Card* curr_node = curr_col->head;
    for (int j = 0; j < targetRow; j++) {
        if (curr_node->next == curr_col->tail) {
            if (j == targetRow - 1) {
                break;
            } else {
                printf("_____________________\nThere are not enough cards in col %d to add a card at row %d.\n_____________________\n",targetCol, targetRow);
                return;
            }
        }
        curr_node = curr_node->next;

    }
    card->next = curr_node->next;
    card->prev = curr_node;
    curr_node->next->prev = card;
    curr_node->next = card;
}

cardLocation locateSpecificCard(const Board* board, const char* id) {
    cardLocation result = { .card = NULL, .col = 0, .row = 0};
    if (boardHasCard(board)) {
        int colIdx = 0, rowIdx = 0;
        for (const Column* col = board->head->next; col != board->tail; col = col->next) {
            colIdx++;
            rowIdx = 0;
            for (Card* temp = col->head->next; temp != col->tail; temp = temp->next) {
                rowIdx++;
                if (temp->rank == id[0] && temp->suit == id[1]) {
                    result.card = temp;
                    result.col = colIdx;
                    result.row = rowIdx;
                    return result;
                }
            }

        }
    } else {
        printf("Unable to locate card as the board is empty\n");
        return result;
    }

    return result;
}

void showAll(const Board* board) {
    if (boardHasCard(board)) {
        for (const Column* col = board->head->next; col != board->tail; col = col->next) {
            for (Card* temp = col->head->next; temp != col->tail; temp = temp->next) {
                temp->faceUp = true;
            }
        }
        setMessage(board, "OK");
    } else {
        setMessage(board, "No deck is loaded");
    }

}

void flattenBoard(const Board* board, Card* deck[52]) {
    int i = 0;
    int row = 0;
    while (i < 52) {
        int cardsThisRow = 0;

        for (const Column* col = board->head->next; col != board->tail; col = col->next) {
            Card* temp = col->head->next;
            int tempRow = 0;
            while (tempRow < row && temp != col->tail) {
                temp = temp->next;
                ++tempRow;
            }
            if (temp != col->tail) {
                deck[i++] = temp;
                ++cardsThisRow;
            }
        }
        if (cardsThisRow == 0) {
            break;
        }
        ++row;
    }
}

void moveDeck(const Board* board, Card* deck[52]) {
    int row = 0, colIdx = 1;
    for (int i = 0; i < 52; ++i) {
        moveCard(board, deck[i], colIdx, row+1);

        if (++colIdx > 7) {
            colIdx = 1;
            ++row;
        }
    }
}

void splitShuffle(const Board* board, int split) {
    Card* deck[52]= {NULL};
    if (boardHasCard(board)) {
        flattenBoard(board, deck);
        if (split < 0 || split > 52 || !split) split = rand() % 51 + 1;
        Card* leftSplit[52];
        Card* rightSplit[52];
        const int leftLen = split;
        const int rightLen = 52 - split;
        memcpy(leftSplit, deck, leftLen * sizeof(Card*));
        memcpy(rightSplit, deck + leftLen, rightLen * sizeof(Card*));
        int leftIdx = 0, rightIdx = 0, deckIdx = 0;
        while (leftIdx < leftLen || rightIdx < rightLen) {
            if (leftIdx < leftLen) deck[deckIdx++] = leftSplit[leftIdx++];
            if (rightIdx < rightLen) deck[deckIdx++] = rightSplit[rightIdx++];
        }
        printf(deck);
        moveDeck(board, deck);
        setMessage(board, "OK");
    } else {
        setMessage(board, "No deck is loaded");
    }
}

void shuffleRandom(Board* board) {
    srand((unsigned)&board + (unsigned)clock());
    Card* deck[52] = {NULL};
    if (boardHasCard(board)) {
        flattenBoard(board, deck);
        Card* shuffledDeck[52] = {NULL};
        int size = 0;
        for (int i = 0; i < 52; ++i) {
            int pos = rand() % (size +1);
            if (size > pos) {
                memmove(shuffledDeck + pos + 1, shuffledDeck + pos, (size - pos) * sizeof(Card*));
            }
            shuffledDeck[pos] = deck[i];
            ++size;
        }
        moveDeck(board, shuffledDeck);
    } else {
        printf("Unable to shuffle as the board is empty\n");
    }
}

void saveDeck(const Board* board, const char* path) {
    Card* deck[52] = {NULL};
    FILE* file = NULL;
    if (boardHasCard(board)) {
        flattenBoard(board, deck);
            if (path) {
                file = fopen(path, "w");
                fprintf(file, "%c%c\n", deck[0]->rank, deck[0]->suit);
                file = fopen(path, "a");
                for (int i = 1; i < 52; ++i) {
                    fprintf(file, "%c%c\n", deck[i]->rank, deck[i]->suit);
                }
            } else {
                file = fopen("cards.txt", "w");
                fprintf(file, "%c%c\n", deck[0]->rank, deck[0]->suit);
                file = fopen("cards.txt", "a");
                for (int i = 1; i < 52; ++i) {
                    fprintf(file, "%c%c\n", deck[i]->rank, deck[i]->suit);
                }
            }
        setMessage(board, "OK");
    } else {
        setMessage(board, "No deck is loaded");
    }
}

void exitProgram(const Board* board) {
    if (boardHasCard(board)) {
        emptyBoard(board);
    }
    exit(0);
}

int findMaxRows(const Board* board) {
    int max = 0;
        for (const Column* col = board->head->next; col != board->tail; col = col->next) {
            int rows = 0;
            for (Card* card = col->head->next; card != col->tail; card = card->next) {
                ++rows;
            }
            if (rows > max) {
                max = rows;
            }
        }
    return max;
}

void displayBoardState(const Board* board) {
    int maxRows = findMaxRows(board);
    if (maxRows < 7) {
        maxRows = 7;
    }

    printf("c1\tc2\tc3\tc4\tc5\tc6\tc7\n\n");
    for (int row = 0; row < maxRows; ++row) {
        for (const Column* col = board->head->next; col != board->tail; col = col->next) {
            const Card* card = col->head->next;
            int r = 0;
            while (r<row && card != col->tail) {
                card = card->next;
                ++r;
            }
            if (card != col->tail) {
                if (card->faceUp == true) {
                    printf("%c%c\t", card->rank, card->suit);
                } else {
                    printf("[]\t");
                }
            } else {
                printf("  \t");
            }

        }
        if (row%2 == 0 && row/2 < 4) {
            putchar('\t');
            int fIdx = row/2;
            Foundation* f = board->foundationHead->next;
            for (int i = 0;i < fIdx && f != board->foundationTail; ++i) {
                f = f->next;
            }
            const Card* top = (f != board->foundationTail && f->tail->prev != f->head ) ? f->tail->prev : NULL;
            if (top) {
                putchar(top->rank); putchar(top->suit); putchar('\t'); putchar('F'); putchar('1'+fIdx);
            } else {
                putchar('['); putchar(']'); putchar('\t'); putchar('F'); putchar('1'+fIdx);
            }

        }
        putchar('\n');
    }
}

void displayBoard(const Board* board, char* lastCommand) {
    printf("\033[H\033[2J\n");
    displayBoardState(board);
    printf("\nLAST Command: %s\n", lastCommand);
    if (board->msg[0] != '\0') {
        printf("Message: %s\n", board->msg);
    } else {
        printf("Message:\n");
    }
    printf("INPUT > ");
}

void commandCenter(Board* board, char* input) {

    char copy[128];
    char* lastCommand = NULL;
    strncpy(copy, input, 128);
    copy[sizeof(copy)-1] = '\0';

    copy[strcspn(copy, "\n")] = '\0';
    for (char* c = copy; *c; ++c) {
        *c = (char)toupper((unsigned char)*c);
    }

    char* cmd = copy;
    char* rest = cmd + strcspn(cmd, " \t");

    if (*rest) {
        *rest++ = '\0';
        while (*rest == ' ' || *rest == '\t') {
            ++rest;
        }
    } else {
        rest = NULL;
    }

    if (strcmp(cmd, "LD") == 0) {
        emptyBoard(board);
        loadFromFile(rest, board);
    }

    if (strcmp(cmd, "SW") == 0) {
        showAll(board);
    }
    if (strcmp(cmd, "SI") == 0) {
        splitShuffle(board, atoi(rest));
    }
    if (strcmp(cmd, "SR") == 0) {
        shuffleRandom(board);
    }
    if (strcmp(cmd, "SD") == 0) {
        saveDeck(board, rest);
    }
    if (strcmp(cmd, "QQ") == 0) {
        exitProgram(board);
    }
    lastCommand = copy;
    displayBoard(board, lastCommand);
}