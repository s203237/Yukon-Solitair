import pygame
import re

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

def parse_cards(text):
    lines = [line.strip() for line in text.splitlines() if line.strip()]

    columns = []
    # Only parse the first 7 lines as columns
    for col_idx, line in enumerate(lines[:7], start=1):
        column = []
        tokens = line.split()
        for row_idx, tok in enumerate(tokens, start=1):
            # Match face-up [RS] or face-down []
            m = re.fullmatch(r'\[(?:(?P<r>[A23456789TJQK])(?P<s>[CDHS])|)\]', tok)
            if not m:
                continue
            if m.group('r') and m.group('s'):
                column.append(Card(m.group('s'), m.group('r'), col_idx*100-25, 50 + row_idx*30, True))
            else:
                column.append(Card('','', col_idx*100-25, 50 + row_idx*30, False))
        columns.append(column)
    foundations = []
    for found_idx, line in enumerate(lines[7:], start=1):
        m = re.fullmatch(r'\[(?:(?P<r>[A23456789TJQK])(?P<s>[CDHS])|)\]', line)
        if not m:
            continue
        if m.group('r') and m.group('s'):
            foundations.append(Card(m.group('s'), m.group('r'), 1000, found_idx*100-20, True))
        else:
            foundations.append(Card(' ',' ', 1000, found_idx*100-20, False))

    return columns, foundations