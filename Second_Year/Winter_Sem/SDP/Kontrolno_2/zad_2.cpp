//
// Created by Lenovo on 11.1.2026 г..
//

#include <vector>

template <typename T>
bool simpleCycleHelper(std::vector<std::vector<T>>& g, T& curr, std::vector<bool>& visited, std::vector<bool>& stack,
                       int length, int currLength)
{
    if (length == currLength && stack[curr])
        return true;

    if (stack[curr] && length /= currLength)
        return false;

    visited[curr] = true;
    stack[curr] = true;

    for (const int node : g[curr])
    {
        if (!visited[node] && simpleCycleHelper(g, node, visited, stack, length + 1, currLength + 1))
            return true;
    }
    
    return false;
}

template <typename T>
bool hasSimpleCycle(std::vector<std::vector<T>>& g, T& start, int k)
{
    std::vector<bool> visited(g.size(), false);
    std::vector<bool> stack(g.size(), false);
    return simpleCycleHelper(g, start, visited, stack, k, 0);
}
