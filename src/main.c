
/*#include "game.c"
#include <stdio.h>
#include <string.h>*/
#include "linked_list_test.c"
//#include "card.c"

int main() {
    Board* board = initBoard();
    /*printf("Board initialized\n");
    Card* sampleCard = createCard('T', 'C', false);
    Card* sampleCard2 = createCard('J', 'C', false);
    printf("card created: %c of %c\n", sampleCard->rank, sampleCard->suit);
    int status = addCard(board, sampleCard, 4, 1);
    if (status) {
        printf("card added\n");
    }
    status = addCard(board, sampleCard2, 4, 2);
    if (status) {
        printf("card added\n");
    }
    //printf("%c, %c exists\n", board->head->next->next->next->next->head->next->next->rank, board->head->next->next->next->next->head->next->next->suit);
    printf("%s\n", boardHasCard(board) ? "true" : "false");
    emptyBoard(board);
    printf("board should be empty\n");
    printf("%s\n", boardHasCard(board) ? "true" : "false");
    return 0;*/
    loadFromFile("src/cards.txt", board);
    splitShuffle(board, 26);
    cardLocation loc = locateSpecificCard(board, "AC");
    printf("%c%c located at col %i, row %i, %s\n", loc.card->rank,loc.card->suit, loc.col, loc.row, loc.card->faceUp ? "true" : "false");
    loc = locateSpecificCard(board, "2C");
    printf("%c%c located at col %i, row %i\n", loc.card->rank,loc.card->suit, loc.col, loc.row);
    loc = locateSpecificCard(board, "AH");
    printf("%c%c located at col %i, row %i, %s\n", loc.card->rank,loc.card->suit, loc.col, loc.row, loc.card->faceUp ? "true" : "false");
    loc = locateSpecificCard(board, "2H");
    printf("%c%c located at col %i, row %i\n", loc.card->rank,loc.card->suit, loc.col, loc.row);

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

