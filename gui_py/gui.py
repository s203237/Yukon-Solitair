import pygame
import sys
from Cards import parse_cards, Card

pygame.init()

WIDTH, HEIGHT = 1200, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Yukon Solitaire")
clock = pygame.time.Clock()

# Background color
BG_COLOR = (40, 80, 120)

running = True
while running:
    screen.fill(BG_COLOR)
    with open("trustThisIsASocket.txt", 'r') as f:
        text = f.read()
    columns, foundations = parse_cards(text)

    # Draw all cards
    for column in columns:
        for card in column:
            card.draw(screen)

    for foundation in foundations:
        foundation.draw(screen)
    #Events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False



    pygame.display.flip()
    clock.tick(4)

pygame.quit()
sys.exit()