//
// Created by Lenovo on 24.1.2026 г..
//

#include <iostream>
#include <queue>
#include <string>

void printFirstKBinary(const unsigned n)
{
    std::queue<std::string> q;
    q.push("1");

    int temp = 1;
    do
    {
        std::string curr = q.front();
        q.pop();

        std::cout << curr << std::endl;

        q.push(curr + '0');
        q.push(curr + '1');

        ++temp;
    }
    while (temp <= n);
}

int main()
{
    unsigned int n;
    std::cin >> n;

    printFirstKBinary(n);

    return 0;
}
