//
// Created by Lenovo on 28.1.2026 г..
//

#include <queue>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> binary(unsigned n)
{
    std::vector<std::string> result;
    if (n < 1) return result;

    std::queue<std::string> q;
    q.push("1");

    for (unsigned i = 0; i < n; ++i)
    {
        std::string front = q.front();
        q.pop();

        q.push(front + '0');
        q.push(front + '1');

        result.push_back(std::move(front));
    }

    return result;
}

int main()
{
    const std::vector<std::string> res = binary(2048);

    for (std::string const& s : res)
        std::cout << s << std::endl;

    return 0;
}
