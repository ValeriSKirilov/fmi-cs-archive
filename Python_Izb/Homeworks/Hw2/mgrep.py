import re
import argparse
import shutil
import os
import requests
from multiprocessing import Pool


class InvalidFileError(Exception):
    def __init__(self):
        super().__init__("Invalid file path")


class InvalidAmountOfWorkers(Exception):
    def __init__(self):
        super().__init__("Invalid amount of workers")


def search_in_file(pattern: str, file_path: str, is_in_memory: bool) -> list[tuple[str, str, int]]:
    try:
        with open(file_path) as fd:
            result = []
            line_number = 1

            if is_in_memory:
                content = fd.readlines()
                for line in content:
                    if re.search(pattern, line):
                        result.append((line.strip(), file_path, line_number))

                    line_number += 1
            else:
                for line in fd:
                    if re.search(pattern, line):
                        result.append((line.strip(), file_path, line_number))
                    line_number += 1

        return result

    except FileNotFoundError:
        raise InvalidFileError()


def run_multi_threaded(pattern: str, files: list[str], is_in_memory: bool, is_line_numbers: bool,
                       amount_of_workers: int) -> None:
    if amount_of_workers < 0:
        raise InvalidAmountOfWorkers()

    if amount_of_workers > 0 and amount_of_workers > len(files):
        raise InvalidAmountOfWorkers()

    if amount_of_workers == 0:
        for file_path in files:
            current = search_in_file(pattern, file_path, is_in_memory)

            if is_line_numbers:
                for line in current:
                    print(f"{line[1]}:{line[2]} - {line[0]}")
            else:
                for line in current:
                    print(line[0])

    else:
        with Pool(processes=amount_of_workers) as pool:
            results = pool.starmap(search_in_file, [(pattern, file_path, is_in_memory) for file_path in files])
            for result in results:
                if is_line_numbers:
                    for line in result:
                        print(f"{line[1]}:{line[2]} - {line[0]}")
                else:
                    for line in result:
                        print(line[0])


def get_files_recursively() -> list[str]:
    result = []

    for dirpath, dirnames, filenames in os.walk('..'):
        for filename in filenames:
            result.append(os.path.join(dirpath, filename))

    return result


def get_files_from_url(urls: list[str]) -> list[str]:
    result = []

    os.makedirs('temp', exist_ok=True)

    for url in urls:
        response = requests.get(url)

        if response.status_code != 200:
            print("Error when reading file.")
            continue

        file_path = os.path.join('temp', url.split("/")[-1])
        with open(file_path, 'w') as fd:
            fd.write(response.text)

        result.append(file_path)

    return result


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('pattern',
                        help='Pattern to search for')
    parser.add_argument('files', nargs='*',
                        help='Files to search in')

    parser.add_argument('-n', '--line-number', action='store_true',
                        help='Show the numbers of lines with a match')
    parser.add_argument('-m', '--in-memory', action='store_true',
                        help='Load the whole file into the memory before searching')
    parser.add_argument('-p', '--parallel', type=int,
                        help='Number of parallel workers to use')

    group = parser.add_mutually_exclusive_group()
    group.add_argument('-r', '--recursive', action='store_true',
                       help='Search recursively in subdirectories')
    group.add_argument('-u', '--from-url', action='store_true',
                       help='Search in files from URL addresses')

    args = parser.parse_args()

    if args.recursive:
        files_to_search = get_files_recursively()

    elif args.from_url:
        files_to_search = get_files_from_url(args.files)

    else:
        files_to_search = args.files

    try:
        run_multi_threaded(pattern=args.pattern, files=files_to_search, is_in_memory=args.in_memory,
                           is_line_numbers=args.line_number, amount_of_workers=args.parallel or 0)

    finally:
        if os.path.exists('temp'):
            shutil.rmtree('temp')
