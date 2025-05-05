//
// Created by Iryna Lutsyk on 05/05/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "game.h"   // <- Din egen spilmotor
#include "card.h"// <- Kortstruktur (Card*)
#include "io.h"
#include "utils.h"  // <- evt. hjælpefunktioner

#define PORT 12345

int main() {
    // Initialiser spillet
    initGame();
    deck = createFullDeck();  // Hvis du har en funktion til dette
    shuffleDeckRandom();
    dealCards();
    showAllCards();  // Vend alle kort op (kan fjernes hvis du bruger regler)

    // Socket setup
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 1);

    printf("[C-server] Waiting for connection...\n");

    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    printf("[C-server] Connection created!\n");

    // Hovedloop: modtag kommandoer
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t read_bytes = read(client_fd, buffer, sizeof(buffer));
        if (read_bytes <= 0) break;

        printf("[C-server] Get: %s", buffer);

        if (strncmp(buffer, "GET", 3) == 0) {
            const char* board = getBoardString();
            send(client_fd, board, strlen(board), 0);
        } else {
            const char* unknown = "UNKNOWN COMMAND\n";
            send(client_fd, unknown, strlen(unknown), 0);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
