import pygame
import sys

pygame.init()

# Vinduesstørrelse
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Yukon Solitaire")

# Farver
BG_COLOR = (40, 80, 120)

# FPS
clock = pygame.time.Clock()

# Hovedloop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill(BG_COLOR)
    pygame.display.flip()
    clock.tick(60)

pygame.quit()
sys.exit()

dragging = False
card_pos = [100, 100]

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            mx, my = pygame.mouse.get_pos()
            if card_rect.collidepoint(mx, my):
                dragging = True
                offset_x = card_pos[0] - mx
                offset_y = card_pos[1] - my

        elif event.type == pygame.MOUSEBUTTONUP:
            dragging = False

        elif event.type == pygame.MOUSEMOTION and dragging:
            mx, my = pygame.mouse.get_pos()
            card_pos = [mx + offset_x, my + offset_y]

    screen.fill(BG_COLOR)
    card_rect = screen.blit(card_img, card_pos)
    pygame.display.flip()
    clock.tick(60)
