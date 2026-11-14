import requests
from .models import Genre, Movie, Series, GenreQuery
from .exceptions import (
    TMDBException,
    TMDBAuthenticationError,
    TMDBNotFoundError,
    TMDBRateLimitError,
    TMDBServerError
)


class TMDBClient:
    def __init__(self, api_key: str):
        if api_key is None or api_key == "" or not api_key:
            raise ValueError("API key is required")
        self.api_key = api_key
        self.session = requests.Session()

    def _check_for_errors(self, response: requests.Response) -> None:
        if response.status_code == 401:
            raise TMDBAuthenticationError("Invalid API key")

        if response.status_code == 404:
            raise TMDBNotFoundError("Resource not found")

        if response.status_code == 429:
            raise TMDBRateLimitError("Rate limit exceeded")

        if response.status_code >= 500:
            raise TMDBServerError("Internal server error")

        if not (200 <= response.status_code < 300):
            raise TMDBException(f"Request failed with status code {response.status_code}")

    def _build_params(self, base_params: dict, **optional_params) -> dict:
        params = base_params.copy()
        for key, value in optional_params.items():
            if value is not None:
                params[key] = value
        return params

    def discover_movies(
            self,
            language: str | None = None,
            region: str | None = None,
            sort_by: str | None = None,
            year: int | None = None,
            genres: Genre | GenreQuery | None = None,
            limit: int = 10,
    ) -> list[Movie]:
        if limit <= 0:
            raise ValueError("Limit must be positive")

        genres_param = None
        if genres is not None:
            if isinstance(genres, Genre):
                genres_param = str(genres.id)
            elif isinstance(genres, GenreQuery):
                genres_param = str(genres)
            else:
                raise TypeError("genres must be Genre, GenreQuery, or None")

        movies = []
        page = 1

        while len(movies) < limit:
            response = self.session.get(
                "https://api.themoviedb.org/3/discover/movie",
                params=self._build_params(
                    {"api_key": self.api_key, "page": page},
                    language=language,
                    region=region,
                    sort_by=sort_by,
                    year=year,
                    with_genres=genres_param
                )
            )

            self._check_for_errors(response)
            data = response.json()

            if page > data.get('total_pages', 0):
                break

            movies_list = data['results']

            if not movies_list:
                break

            for movie_dict in movies_list:
                if len(movies) >= limit:
                    break

                movie = Movie(
                    id=movie_dict['id'],
                    title=movie_dict['title'],
                    vote_average=movie_dict.get('vote_average', 0.0),
                    original_title=movie_dict.get('original_title'),
                    overview=movie_dict.get('overview'),
                    release_date=movie_dict.get('release_date'),
                    poster_path=movie_dict.get('poster_path'),
                    backdrop_path=movie_dict.get('backdrop_path'),
                    genre_ids=movie_dict.get('genre_ids'),
                    popularity=movie_dict.get('popularity'),
                    vote_count=movie_dict.get('vote_count'),
                    adult=movie_dict.get('adult'),
                    original_language=movie_dict.get('original_language'),
                    video=movie_dict.get('video')
                )
                movies.append(movie)

            page += 1

        return movies[:limit]

    def discover_series(
            self,
            language: str | None = None,
            sort_by: str | None = None,
            genres: Genre | GenreQuery | None = None,
            limit: int = 10,
    ) -> list[Series]:
        if limit <= 0:
            raise ValueError("Limit must be positive")

        genres_param = None
        if genres is not None:
            if isinstance(genres, Genre):
                genres_param = str(genres.id)
            elif isinstance(genres, GenreQuery):
                genres_param = str(genres)
            else:
                raise TypeError("genres must be Genre, GenreQuery, or None")

        series = []
        page = 1

        while len(series) < limit:
            response = self.session.get(
                "https://api.themoviedb.org/3/discover/tv",
                params=self._build_params(
                    {"api_key": self.api_key, "page": page},
                    language=language,
                    sort_by=sort_by,
                    with_genres=genres_param
                )
            )

            self._check_for_errors(response)
            data = response.json()

            if page > data.get('total_pages', 0):
                break

            series_list = data['results']

            if not series_list:
                break

            for series_dict in series_list:
                if len(series) >= limit:
                    break

                tv = Series(
                    id=series_dict['id'],
                    name=series_dict['name'],
                    vote_average=series_dict.get('vote_average', 0.0),
                    original_name=series_dict.get('original_name'),
                    overview=series_dict.get('overview'),
                    first_air_date=series_dict.get('first_air_date'),
                    poster_path=series_dict.get('poster_path'),
                    backdrop_path=series_dict.get('backdrop_path'),
                    genre_ids=series_dict.get('genre_ids'),
                    popularity=series_dict.get('popularity'),
                    vote_count=series_dict.get('vote_count'),
                    original_language=series_dict.get('original_language')
                )
                series.append(tv)

            page += 1

        return series[:limit]

    def get_movie_genres(
            self,
            language: str | None = None,
    ) -> list[Genre]:
        response = self.session.get(
            "https://api.themoviedb.org/3/genre/movie/list",
            params=self._build_params(
                {"api_key": self.api_key},
                language=language
            )
        )

        self._check_for_errors(response)

        data = response.json()

        genres_list = data['genres']
        genres = []
        for genre_dict in genres_list:
            genre = Genre(
                id=genre_dict['id'],
                name=genre_dict['name']
            )
            genres.append(genre)

        return genres

    def get_series_genres(
            self,
            language: str | None = None,
    ) -> list[Genre]:
        response = self.session.get(
            "https://api.themoviedb.org/3/genre/tv/list",
            params=self._build_params(
                {"api_key": self.api_key},
                language=language
            )
        )

        self._check_for_errors(response)

        data = response.json()

        genres_list = data['genres']
        genres = []
        for genre_dict in genres_list:
            genre = Genre(
                id=genre_dict['id'],
                name=genre_dict['name']
            )
            genres.append(genre)

        return genres

    def search_movies(
            self,
            query: str,
            language: str | None = None,
            year: int | None = None,
            limit: int = 10,
    ) -> list[Movie]:
        if limit <= 0:
            raise ValueError("Limit must be positive")

        movies = []
        page = 1

        while len(movies) < limit:
            response = self.session.get(
                "https://api.themoviedb.org/3/search/movie",
                params=self._build_params(
                    {"api_key": self.api_key, "page": page, "query": query},
                    language=language,
                    year=year
                )
            )

            self._check_for_errors(response)
            data = response.json()

            if page > data.get('total_pages', 0):
                break

            movies_list = data['results']

            if not movies_list:
                break

            for movie_dict in movies_list:
                if len(movies) >= limit:
                    break

                movie = Movie(
                    id=movie_dict['id'],
                    title=movie_dict['title'],
                    vote_average=movie_dict.get('vote_average', 0.0),
                    original_title=movie_dict.get('original_title'),
                    overview=movie_dict.get('overview'),
                    release_date=movie_dict.get('release_date'),
                    poster_path=movie_dict.get('poster_path'),
                    backdrop_path=movie_dict.get('backdrop_path'),
                    genre_ids=movie_dict.get('genre_ids'),
                    popularity=movie_dict.get('popularity'),
                    vote_count=movie_dict.get('vote_count'),
                    adult=movie_dict.get('adult'),
                    original_language=movie_dict.get('original_language'),
                    video=movie_dict.get('video')
                )
                movies.append(movie)

            page += 1

        return movies[:limit]

    def search_series(
            self,
            query: str,
            language: str | None = None,
            limit: int = 10,
    ) -> list[Series]:
        if limit <= 0:
            raise ValueError("Limit must be positive")

        series = []
        page = 1

        while len(series) < limit:
            response = self.session.get(
                "https://api.themoviedb.org/3/search/tv",
                params=self._build_params(
                    {"api_key": self.api_key, "page": page, "query": query},
                    language=language
                )
            )

            self._check_for_errors(response)
            data = response.json()

            if page > data.get('total_pages', 0):
                break

            series_list = data['results']

            if not series_list:
                break

            for series_dict in series_list:
                if len(series) >= limit:
                    break

                serie = Series(
                    id=series_dict['id'],
                    name=series_dict['name'],
                    vote_average=series_dict.get('vote_average', 0.0),
                    original_name=series_dict.get('original_name'),
                    overview=series_dict.get('overview'),
                    first_air_date=series_dict.get('first_air_date'),
                    poster_path=series_dict.get('poster_path'),
                    backdrop_path=series_dict.get('backdrop_path'),
                    genre_ids=series_dict.get('genre_ids'),
                    popularity=series_dict.get('popularity'),
                    vote_count=series_dict.get('vote_count'),
                    original_language=series_dict.get('original_language')
                )
                series.append(serie)

            page += 1

        return series[:limit]
