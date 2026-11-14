from enum import Enum, auto


class Suit(Enum):
    CLUBS = '\u2663'
    DIAMONDS = '\u2666'
    HEARTS = '\u2665'
    SPADES = '\u2660'
    WILD = 'W'


class Card:
    CHIP_VALUES = {'2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9, '10': 10, 'J': 10, 'Q': 10,
                   'K': 10, 'A': 11}

    def __init__(self, rank, suit):
        self._rank = rank
        self._suit = suit

    @property
    def rank(self):
        return self._rank

    @property
    def suit(self):
        return self._suit

    @property
    def chips(self):
        return self.CHIP_VALUES.get(self._rank)

    def __str__(self):
        return f'{self.rank}{self.suit.value}'


class SilverCard(Card):
    def __init__(self, rank, suit):
        super().__init__(rank, suit)

    @property
    def chips(self):
        return 2 * super().chips

    def __str__(self):
        return f'Silver {super().__str__()}'


class GoldCard(Card):
    def __init__(self, rank, suit):
        super().__init__(rank, suit)

    @property
    def chips(self):
        return 4 * super().chips

    def __str__(self):
        return f'Gold {super().__str__()}'


class WildCard(Card):
    def __init__(self, rank):
        super().__init__(rank, Suit.WILD)

    def __str__(self):
        return f'Wild {super().__str__()}'


class Joker:
    def _default_action(self, current_chips, current_mult):
        return current_chips, current_mult

    def __init__(self, chips, mult, action=None):
        self._chips = chips
        self._mult = mult

        if action is None:
            self._action = self._default_action
        else:
            self._action = action

    @property
    def chips(self):
        return self._chips

    @property
    def mult(self):
        return self._mult

    @property
    def action(self):
        return self._action


def score(cards, jokers=None):
    if len(cards) != 5:
        return 0

    if jokers is None:
        jokers = []

    chips = 0
    mult = 1

    counts = {'2': 0, '3': 0, '4': 0, '5': 0, '6': 0, '7': 0, '8': 0, '9': 0, '10': 0,
              'J': 0, 'Q': 0, 'K': 0, 'A': 0,
              Suit.CLUBS: 0, Suit.DIAMONDS: 0, Suit.HEARTS: 0, Suit.SPADES: 0}

    for card in cards:
        chips += card.chips

        counts[card.rank] += 1
        if card.suit != Suit.WILD:
            counts[card.suit] += 1
        else:
            counts[Suit.CLUBS] += 1
            counts[Suit.DIAMONDS] += 1
            counts[Suit.HEARTS] += 1
            counts[Suit.SPADES] += 1

    for count in counts.values():
        if count > 1:
            mult += count

    for joker in jokers:
        chips += joker.chips
        mult += joker.mult

    for joker in jokers:
        chips, mult = joker.action(chips, mult)

    return chips * mult
