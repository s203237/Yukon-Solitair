//
// Created by Iryna Lutsyk on 05/05/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 1);

    printf("[C-server] Waiting for connection...\n");

    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    printf("[C-server] Connection created!\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t read_bytes = read(client_fd, buffer, sizeof(buffer));
        if (read_bytes <= 0) break;

        printf("[C-server] Get: %s", buffer);

        if (strncmp(buffer, "GET", 3) == 0) {
            // Return 7 columns with kort en text format.
            const char* game_state =
                "AH 2D 3C\n"
                "4S 5H\n"
                "6D\n"
                "7C 8H 9S\n"
                "10D JD\n"
                "QC KH\n"
                "KS\n";

            send(client_fd, game_state, strlen(game_state), 0);
        } else {
            const char* unknown = "UNKNOWN COMMAND\n";
            send(client_fd, unknown, strlen(unknown), 0);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

