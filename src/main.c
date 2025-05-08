#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "io.h"
#include "game.h"



int main() {
    Board*board = initBoard();
    Card* deck = NULL;
    printBoard(board);
    char input[100];// save user input command
    char lastCommand[100] = "";//display previous command
    char message[100] = "Welcome to Yukon!"; //notify execution result
    bool inPlayPhase = false;
    

    while (1) {
        printf("\nLAST Command: %s\n", lastCommand);
        printf("Message: %s\n", message);
        printf("INPUT > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        strcpy(lastCommand, input);

        if (strncmp(input, "LD ", 3) == 0) {
            if (loadDeckFromFile(input + 3)) {
                dealDeckForGridView(board,deck);
                strcpy(message, "Deck loaded successfully.");
                printBoard(board);
            } else {
                strcpy(message, "Error loading deck.");
            }
        } else if (strcmp(input, "SW") == 0) {
            showAllCards(board);
            strcpy(message, "Showing all cards.");
            printBoard(board);
       
        }else if (strncmp(input, "SI", 3) == 0) {
                if (inPlayPhase) strcpy(message, "Command not available in PLAY phase.");
                else {
                    int split = atoi(input + 3);
                    shuffleDeckSplit(deck,split);
                    strcpy(message, "Deck split-shuffled.");
                }
        }else if (strcmp(input, "SR") == 0) {
                if (inPlayPhase) strcpy(message, "Command not available in PLAY phase.");
                else {
                    shuffleDeckRandom(deck);
                    dealCardsForPlayPhase(board,deck); 
                    printBoard(board);
                    strcpy(message, "Deck shuffled randomly.");
                }
        } else if (strcmp(input, "P") == 0) {
            dealCardsForPlayPhase(board,deck); 
            inPlayPhase = true;
            printBoard(board);
            strcpy(message, "Cards dealt to columns.");
        } else if (strcmp(input, "QQ") == 0) {
            printf("Bye!\n");
            break;
        } else {
            strcpy(message, "Unknown command.");
        }
    }

    return 0;
}

// #include <stdio.h>
// #include "game.c"

 //int main() {
    //printBoard();
     //return 0;
 //}

