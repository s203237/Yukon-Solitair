#include "linked_list_test.c"

int main() {

    Board* board = initBoard();
    board->GUI = false;
    commandCenter(board, " ");
        while (1) {
            char input[128];
            fflush(stdout);

            if (!fgets(input, sizeof input, stdin)) {
                break;
            }
            commandCenter(board, input);
            if (board->GUI) {
                saveBoardState(board);
            }
        }

}