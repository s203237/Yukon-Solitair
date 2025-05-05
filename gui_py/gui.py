import pygame
import sys
from Cards import parse_cards, Card
from Client import connect_to_server, send_command  # <- socket-client

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
columns = parse_cards(response)

# Mouse state
selected_card = None
dragging = False
offset_x = 0
offset_y = 0

# Main loop
running = True
while running:
    screen.fill(BG_COLOR)

    # Draw all cards
    for column in columns:
        for card in column:
            card.draw(screen)
    #Events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            mx, my = pygame.mouse.get_pos()

            #Go backwords through cards to find the top card under the mouse.
            for column in reversed(columns):
                for card in reversed(column):
                    if card.rect.collidepoint(mx, my) and card.fae:up:
                    selected_card = card
                    offset_x = mx - card.rect.x
                    offset_y = my - card.rect.y
                    dragging = True
                    break
                if dragging:
                    break

        elif event.type == pygame.MOUSEBUTTONUP:
            dragging = False
            selected_card = None # (MOVE server)

        elif event.type == pygame.MOUSEMOTION and dragging and selected_card:
            mx, my = pygame.mouse.get_pos()
            selected_card.rect.x = mx - offset_x
            selected_card.rect.y = my - offset_y



    pygame.display.flip()
    clock.tick(60)

pygame.quit()
sys.exit()
