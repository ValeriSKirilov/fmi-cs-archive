from dataclasses import dataclass


@dataclass
class Movie:
    id: int
    title: str
    vote_average: float = 0.0
    original_title: str | None = None
    overview: str | None = None
    release_date: str | None = None
    poster_path: str | None = None
    backdrop_path: str | None = None
    genre_ids: list[int] | None = None
    popularity: float | None = None
    vote_count: int | None = None
    adult: bool | None = None
    original_language: str | None = None
    video: bool | None = None


@dataclass
class Series:
    id: int
    name: str
    vote_average: float = 0.0
    original_name: str | None = None
    overview: str | None = None
    first_air_date: str | None = None
    poster_path: str | None = None
    backdrop_path: str | None = None
    genre_ids: list[int] | None = None
    popularity: float | None = None
    vote_count: int | None = None
    original_language: str | None = None


@dataclass
class Genre:
    id: int
    name: str

    def __and__(self, other):
        if not isinstance(other, Genre):
            raise TypeError("Unsupported operand type(s) for &: 'Genre' and '{}'".format(other.__class__.__name__))

        return GenreQuery([self.id, other.id], 'AND')

    def __or__(self, other):
        if not isinstance(other, Genre):
            raise TypeError("Unsupported operand type(s) for |: 'Genre' and '{}'".format(other.__class__.__name__))

        return GenreQuery([self.id, other.id], 'OR')


@dataclass()
class GenreQuery:
    ids: list[int]
    operator: str

    def __str__(self):
        separator = ',' if self.operator == 'AND' else '|'

        return separator.join(str(id) for id in self.ids)

    def __and__(self, other):
        if isinstance(other, Genre):
            new_ids = self.ids + [other.id]
        elif isinstance(other, GenreQuery):
            new_ids = self.ids + other.ids
        else:
            raise TypeError("Unsupported operand type(s) for &: 'GenreQuery' and '{}'".format(other.__class__.__name__))

        return GenreQuery(new_ids, 'AND')

    def __or__(self, other):
        if isinstance(other, Genre):
            new_ids = self.ids + [other.id]
        elif isinstance(other, GenreQuery):
            new_ids = self.ids + other.ids
        else:
            raise TypeError("Unsupported operand type(s) for |: 'GenreQuery' and '{}'".format(other.__class__.__name__))

        return GenreQuery(new_ids, 'OR')
