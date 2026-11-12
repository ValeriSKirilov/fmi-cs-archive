//
// Created by Lenovo on 28.1.2026 г..
//

#include <queue>
#include <iostream>

int firstUnique(const std::string& s)
{
    int occurrences[256]{};

    std::queue<int> q;
    for (int i = 0; i < s.size(); ++i)
    {
        q.push(i);
        occurrences[s[i]]++;
    }

    while (!q.empty())
    {
        if (const int index = q.front(); occurrences[s[index]] == 1)
            return index;

        q.pop();
    }

    return -1;
}

int main()
{
    std::cout << firstUnique("abcdbac") << std::endl;

    return 0;
}
