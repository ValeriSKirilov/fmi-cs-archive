//
// Created by Lenovo on 24.1.2026 г..
//

#include <iostream>
#include <vector>

std::size_t getUnique(const std::vector<int>& numbers)
{
    std::size_t result = 0;
    std::vector found(numbers.size(), false);

    for (const int num : numbers)
    {
        found[num] = true;
    }

    for (const bool i : found)
        result += i;

    return result;
}

int main()
{
    const std::vector test = {1, 1, 2, 3, 4, 5, 6};
    std::cout << getUnique(test);

    return 0;
}
