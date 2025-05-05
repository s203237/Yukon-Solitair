import pygame
import sys
from Cards import parse_cards, Card
from Client import connect_to_server, send_command  # <- socket-klient

pygame.init()

# Window size and setup
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Yukon Solitaire")
clock = pygame.time.Clock()

# Background color
BG_COLOR = (40, 80, 120)

# Connect to C-server
sock = connect_to_server()

# Get initial game state from C
response = send_command(sock, "GET\n")
columns = parse_cards(response)  # ← brug funktionen fra cards.py

# Main loop
running = True
while running:
    screen.fill(BG_COLOR)

    # Draw all cards
    for column in columns:
        for card in column:
            card.draw(screen)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
sys.exit()
import pygame
import sys
from cards import parse_cards, Card
from client import connect_to_server, send_command  # <- socket-klient

pygame.init()

# Window size and setup
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Yukon Solitaire")
clock = pygame.time.Clock()

# Background color
BG_COLOR = (40, 80, 120)

# Connect to C-server
sock = connect_to_server()

# Get initial game state from C
response = send_command(sock, "GET\n")
columns = parse_cards(response)  # ← brug funktionen fra cards.py

# Main loop
running = True
while running:
    screen.fill(BG_COLOR)

    # Draw all cards
    for column in columns:
        for card in column:
            card.draw(screen)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
sys.exit()
