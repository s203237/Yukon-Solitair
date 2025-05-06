
/*#include "game.c"
#include <stdio.h>
#include <string.h>*/
#include "linked_list_test.c"
//#include "card.c"

int main() {
    Board* board = initBoard();
    commandCenter(board, " ");
    while (1) {
        char input[128];
        fflush(stdout);

        if (!fgets(input, sizeof input, stdin)) {
            break;
        }
        commandCenter(board, input);
    }
    /*commandCenter(board, "LD");
    commandCenter(board, "SW");
    commandCenter(board, "SR");*/
    }

    /*initGame();

    char input[100];// save user input command
    char lastCommand[100] = "";//display previous command
    char message[100] = " "; //notify execution result
    printBoard();

    while (1) {
        printf("\nLAST Command: %s\n", lastCommand);
        printf("Message: %s\n", message);
        printf("INPUT > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        strcpy(lastCommand, input);

        if (strncmp(input, "LD ", 3) == 0) {
            if (loadDeckFromFile(input + 3)) {
                strcpy(message, "Deck loaded successfully.");
            } else {
                strcpy(message, "Error loading deck.");
            }
        } else if (strcmp(input, "SW") == 0) {
            showDeck();
            strcpy(message, "Showing all cards.");
        } else if (strcmp(input, "P") == 0) {
            dealCards();
            printBoard();
            strcpy(message, "Cards dealt to columns.");
        } else if (strcmp(input, "QQ") == 0) {
            printf("Bye!\n");
            break;
        } else {
            strcpy(message, "Unknown command.");
        }
    }

    return 0;*/


// #include <stdio.h>
// #include "game.c"

// int main() {
//     printBoard();
//     return 0;
// }

