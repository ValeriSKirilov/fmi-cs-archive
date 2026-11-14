//
// Created by Lenovo on 28.1.2026 г..
//

#include <iostream>
#include <queue>
#include <algorithm>

struct Positions
{
    int x, y, dist;
};

bool operator==(Positions const& p1, Positions const& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool validStep(const int n, Positions const& p)
{
    return p.x >= 0 && p.x < n && p.y >= 0 && p.y < n;
}

int minSteps(const int n, Positions const& start, Positions const& end)
{
    std::queue<Positions> pos;
    std::vector<std::vector<bool>> visited(n);
    std::fill(visited.begin(), visited.end(), std::vector<bool>(n, false));

    pos.push({start.x, start.y, 0});

    while (!pos.empty())
    {
        Positions curr = pos.front();
        pos.pop();

        if (curr == end)
            return curr.dist;

        if (visited[curr.x][curr.y])
            continue;

        visited[curr.x][curr.y] = true;

        for (int i = -2; i < 2; ++i)
        {
            for (int j = -2; j < 2; ++j)
            {
                if (i != 0 && j != 0 && abs(i) != abs(j))
                {
                    Positions p{curr.x + i, curr.y + j, curr.dist + 1};
                    if (validStep(n, p))
                        pos.push(std::move(p));
                }
            }
        }
    }

    return -1;
}
