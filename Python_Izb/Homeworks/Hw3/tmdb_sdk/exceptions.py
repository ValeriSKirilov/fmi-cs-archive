class TMDBException(Exception):
    pass


class TMDBAuthenticationError(TMDBException):
    pass


class TMDBNotFoundError(TMDBException):
    pass


class TMDBRateLimitError(TMDBException):
    pass


class TMDBServerError(TMDBException):
    pass
