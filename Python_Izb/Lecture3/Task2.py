def number_of_vowels(text):
    vowels = 'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'

    count = 0

    for c in text:
        if c in vowels:
            count += 1

    return count


print(number_of_vowels("grrrrgh!") == 0)
print(number_of_vowels("The quick brown fox jumps over the lazy dog.") == 11)
print(number_of_vowels("MONTHY PYTHON") == 2)
