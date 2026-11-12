//
// Created by Lenovo on 7.1.2026 г..
//

#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

void addEdge(std::vector<std::list<int>>& g, const int v, const int u, const bool isDirected = false)
{
    g[v].push_back(u);

    if (!isDirected)
        g[u].push_back(v);
}

std::vector<std::list<int>> readGraph(const int vertices, const int edges, const bool isDirected = true)
{
    std::vector<std::list<int>> result(vertices);

    for (size_t i = 0; i < edges; i++)
    {
        int from;
        int to;

        std::cin >> from >> to;

        addEdge(result, from, to, isDirected);
    }

    return result;
}

void dfsVisit(const std::vector<std::list<int>>& g, const int curr, std::vector<bool>& visited)
{
    if (visited[curr])
        return;

    visited[curr] = true;

    for (const int node : g[curr])
    {
        if (!visited[node])
        {
            dfsVisit(g, node, visited);
        }
    }
}

void dfs(const std::vector<std::list<int>>& g, const int start)
{
    std::vector<bool> visited(g.size(), false);
    dfsVisit(g, start, visited);
}

int connectedComponentsCount(const std::vector<std::list<int>>& g)
{
    int result = 0;
    std::vector<bool> visited(g.size(), false);

    for (std::size_t i = 0; i < g.size(); i++)
    {
        if (!visited[i])
        {
            dfsVisit(g, i, visited);
            result++;
        }
    }

    return result;
}

void CCAVG_helper(const std::vector<std::list<int>>& g, int curr, int& count, int& sum, std::vector<bool>& visited)
{
    if (visited[curr])
        return;

    visited[curr] = true;
    count++;
    sum += curr;

    for (const int node : g[curr])
    {
        CCAVG_helper(g, node, count, sum, visited);
    }
}

double CCAVG(const std::vector<std::list<int>>& g)
{
    double result = 0;
    std::vector<bool> visited(g.size(), false);

    for (std::size_t i = 0; i < g.size(); i++)
    {
        int count = 0;
        int sum = 0;
        if (!visited[i])
        {
            CCAVG_helper(g, i, count, sum, visited);
            result += static_cast<double>(sum) / count;
        }
    }

    return result;
}

void bfs(const std::vector<std::list<int>>& g, int start)
{
    std::queue<int> q;
    std::vector<bool> visited(g.size(), false);

    q.push(start);
    visited[start] = true;

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        visited[curr] = true;

        for (int node : g[curr])
        {
            if (!visited[curr])
            {
                q.push(node);
                visited[node] = true;
            }
        }
    }
}

std::vector<int> bfsShortestPath(const std::vector<std::list<int>>& g, const int start, const int end)
{
    std::vector<bool> visited(g.size(), false);
    std::vector<int> parent(g.size(), -1);
    std::queue<int> q;
    q.push(start);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();
        visited[curr] = true;

        for (int node : g[curr])
        {
            if (!visited[node])
            {
                q.push(node);
                parent[node] = curr;
                visited[node] = true;
            }
        }

        if (curr == end)
            break;
    }

    std::vector<int> path;
    int curr = end;

    while (curr != -1)
    {
        path.push_back(curr);
        curr = parent[curr];
    }

    std::reverse(path.begin(), path.end());

    return path;
}

void topoSortHelper(const std::vector<std::list<int>>& g, const int curr, std::vector<bool>& visited,
                    std::stack<int>& stack)
{
    if (visited[curr])
        return;

    visited[curr] = true;

    for (const int node : g[curr])
    {
        topoSortHelper(g, node, visited, stack);
    }

    stack.push(curr);
}

std::vector<int> topoSort(const std::vector<std::list<int>>& g)
{
    std::vector<bool> visited(g.size(), false);
    std::stack<int> stack;

    for (std::size_t i = 0; i < g.size(); i++)
    {
        topoSortHelper(g, i, visited, stack);
    }

    std::vector<int> result;
    while (!stack.empty())
    {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}

bool hasCycleHelp(const std::vector<std::list<int>>& g, const int curr, std::vector<bool>& visited,
                  std::vector<bool>& stack)
{
    if (visited[curr])
        return false;

    visited[curr] = true;
    stack[curr] = true;

    for (const int node : g[curr])
    {
        if (stack[node])
            return true;

        if (!visited[node] && hasCycleHelp(g, node, visited, stack))
            return true;
    }

    stack[curr] = false;
    return false;
}

bool hasCycle(const std::vector<std::list<int>>& g)
{
    std::vector<bool> visited(g.size(), false);
    std::vector<bool> stack(g.size(), false);

    for (std::size_t i = 0; i < g.size(); i++)
    {
        if (hasCycleHelp(g, i, visited, stack))
            return true;
    }

    return false;
}

int main()
{
    std::vector<std::list<int>> g(9);

    addEdge(g, 7, 8);
    addEdge(g, 4, 5);
    addEdge(g, 4, 6);
    addEdge(g, 5, 6);
    addEdge(g, 2, 1);
    addEdge(g, 0, 3);
    addEdge(g, 0, 1);
    addEdge(g, 2, 3);

    std::cout << hasCycle(g);

    // std::vector<int> test = bfsShortestPath(g, 4, 6);
    //
    // for (int node : test)
    // {
    //     std::cout << node << " ";
    // }

    // addEdge(g, 0, 2);
    // addEdge(g, 0, 4);
    // addEdge(g, 0, 5);
    // addEdge(g, 0, 8);
    //
    // addEdge(g, 1, 7);
    // addEdge(g, 1, 3);
    //
    // addEdge(g, 2, 4);
    // addEdge(g, 2, 5);
    // addEdge(g, 2, 6);
    // addEdge(g, 2, 7);
    //
    // addEdge(g, 3, 9);
    //
    // addEdge(g, 4, 6);
    //
    // addEdge(g, 8, 11);
    // addEdge(g, 8, 10);

    // std::cout << CCAVG(g);

    return 0;
}
