import pygame
import sys
from cards import Card              # <- Din Card-klasse (vi laver næste)
from client import connect_to_server, send_command  # <- socket-klient

pygame.init()

# Windouws size and set up.
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Yukon Solitaire")
clock = pygame.time.Clock()

# Colors and bagground.
BG_COLOR = (40, 80, 120)

# Connection to C-server.
sock = connect_to_server()

# Funktion par answer from C and make connection.
def parse_cards(response):
    columns = []
    lines = response.strip().split('\n')
    for col_index, line in enumerate(lines):
        column = []
        cards = line.strip().split()
        for row_index, code in enumerate(cards):
            rank = code[:-1]
            suit = code[-1]
            x = 50 + col_index * 100
            y = 50 + row_index * 30
            card = Card(suit, rank, x, y, face_up=True)
            column.append(card)
        columns.append(column)
    return columns

# Get initial game state from C.
response = send_command(sock, "GET\n")
columns = parse_cards(response)

# Main loop.
running = True
while running:
    screen.fill(BG_COLOR)

    # Draw all cards.
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
