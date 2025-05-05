import pygame

# Standard card size and color.
CARD_WIDTH, CARD_HEIGHT = 71, 96
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (200, 0, 0)
GRAY = (120, 120, 120)

class Card:
    FONT = None  # <- class-variabel

    def __init__(self, suit, rank, x, y, face_up=True):
        self.suit = suit  # 'H', 'S', 'C', 'D'
        self.rank = rank  # 'A', '2', ..., 'K'
        self.face_up = face_up
        self.rect = pygame.Rect(x, y, CARD_WIDTH, CARD_HEIGHT)

    def suit_symbol(self):
        return {'S': '♠', 'H': '♥', 'D': '♦', 'C': '♣'}[self.suit]

    def draw(self, surface):
        if Card.FONT is None:
            Card.FONT = pygame.font.SysFont("arial", 20, bold=True)

        color = RED if self.suit in ['H', 'D'] else BLACK

        if self.face_up:
            pygame.draw.rect(surface, WHITE, self.rect, border_radius=6)
            pygame.draw.rect(surface, BLACK, self.rect, 2, border_radius=6)
            text = f"{self.rank}{self.suit_symbol()}"
            label = Card.FONT.render(text, True, color)
            surface.blit(label, (self.rect.x + 8, self.rect.y + 8))
        else:
            pygame.draw.rect(surface, GRAY, self.rect, border_radius=6)
            pygame.draw.rect(surface, BLACK, self.rect, 2, border_radius=6)

# Converts string from C-server to columns of Card objects
def parse_cards(text):
    columns = []
    lines = text.strip().split('\n')
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
