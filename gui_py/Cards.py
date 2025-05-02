import pygame
import sys
import random

pygame.init()
screen = pygame.display.set_mode((1024, 768))
pygame.display.set_caption("Welcome to Yukon Solitaire")
clock = pygame.time.Clock()

# fonts and color definitions
FONT = pygame.font.SysFont("arial", 20, bold=True)
CARD_WIDTH, CARD_HEIGHT = 71, 96
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (200, 0, 0)
GREEN = (0, 128, 0)
GRAY = (120, 120, 120)

#drawing of the cards
def draw_card(surface, card):
    color = RED if card.suit in ['H', 'D'] else BLACK
    if card.face_up:
        pygame.draw.rect(surface, WHITE, card.rect, border_radius=6)
        pygame.draw.rect(surface, BLACK, card.rect, 2, border_radius=6)
        text = f"{card.rank}{card.suit_symbol()}"
        label = FONT.render(text, True, color)
        surface.blit(label, (card.rect.x + 8, card.rect.y + 8))
    else:
        pygame.draw.rect(surface, GRAY, card.rect, border_radius=6)
        pygame.draw.rect(surface, BLACK, card.rect, 2, border_radius=6)

#card class
class Card:
    def __init__(self, suit, rank, x, y, face_up=True):
        self.suit = suit
        self.rank = rank
        self.face_up = face_up
        self.rect = pygame.Rect(x, y, CARD_WIDTH, CARD_HEIGHT)
        self.dragging = False
        self.offset_x = 0
        self.offset_y = 0

    def draw(self, surface):
        draw_card(surface, self)

    def suit_symbol(self):
        return {'S': '♠', 'H': '♥', 'D': '♦', 'C': '♣'}[self.suit]

    #pile class
    class Pile:
        def __init__(self, x, y):
            self.cards = []
            self.x = x
            self.y = y

    def add_card(self, card):
        card.rect.topleft = (self.x, self.y + len(self.cards) * 30)
        self.cards.append(card)

    def draw(self, surface):
        for card in self.cards:
            card.draw(surface)

    def get_card_at_pos(self, pos):
        for card in reversed(self.cards):
            if card.rect.collidepoint(pos):
                return card
        return None

    #create deck
    suits = ['♠', '♥', '♦', '♣']

ranks = ['A'] + [str(n) for n in range(2, 11)] + ['J', 'Q', 'K']
deck = [Card(s, r, 0, 0) for s in suits for r in ranks]
random.shuffle(deck)

#create tables
tableau = [Pile(x * 150 + 50, 100) for x in range(7)]

index = 0
for i, pile in enumerate(tableau):
    for _ in range(i + 1):
        card = deck[index]
        card.face_up = True  # all face up for now
        pile.add_card(card)
        index += 1

