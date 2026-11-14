//
// Created by Lenovo on 26.1.2026 г..
//

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <stack>

void addEdge(std::vector<std::list<int>>& g, int v, int u, bool isDirected = false)
{
    g[v].push_back(u);

    if (!isDirected)
        g[u].push_back(v);
}

std::vector<std::list<int>> readGrapth(int vectices, int edges, bool isDirected = true)
{
    std::vector<std::list<int>> result(vectices);

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
            dfsVisit(g, node, visited);
    }
}

void dfs(const std::vector<std::list<int>>& g, const int start)
{
    std::vector visited(g.size(), false);

    dfsVisit(g, start, visited);
}

int connectedComponentsCount(const std::vector<std::list<int>>& g)
{
    int result = 0;
    std::vector visited(g.size(), false);

    for (std::size_t i = 0; i < g.size(); ++i)
    {
        if (!visited[i])
        {
            dfsVisit(g, i, visited);
            result++;
        }
    }

    return result;
}

void CCAVG_helper(const std::vector<std::list<int>>& g, const int curr, std::vector<bool>& visited, int& sum,
                  int& count)
{
    if (visited[curr])
        return;

    visited[curr] = true;
    count++;
    sum += curr;

    for (const int node : g[curr])
    {
        if (!visited[curr])
            CCAVG_helper(g, node, visited, sum, count);
    }
}

double CCAVG(const std::vector<std::list<int>>& g)
{
    double result = 0;
    std::vector visited(g.size(), false);


    for (std::size_t i = 0; i < g.size(); ++i)
    {
        if (!visited[i])
        {
            int sum = 0, count = 0;
            CCAVG_helper(g, i, visited, sum, count);
            result += static_cast<double>(sum / count);
        }
    }

    return result;
}

void bfs(const std::vector<std::list<int>>& g, const int start)
{
    std::queue<int> q;
    std::vector visited(g.size(), false);

    q.push(start);

    while (!q.empty())
    {
        const int curr = q.front();
        q.pop();
        visited[curr] = true;

        for (const int node : g[curr])
        {
            if (!visited[node])
            {
                q.push(node);
                visited[node] = true;
            }
        }
    }
}

std::vector<int> bfsShortestPath(const std::vector<std::list<int>>& g, const int start, const int end)
{
    std::vector visited(g.size(), false);
    std::vector parent(g.size(), -1);
    std::queue<int> q;
    q.push(start);

    while (!q.empty())
    {
        const int curr = q.front();
        q.pop();
        visited[curr] = true;

        for (const int node : g[curr])
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
    std::vector visited(g.size(), false);
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

        if (!visited[curr] && hasCycleHelp(g, node, visited, stack))
            return true;
    }

    stack[curr] = false;
    return false;
}

bool hasCycle(const std::vector<std::list<int>>& g)
{
    std::vector visited(g.size(), false);
    std::vector stack(g.size(), false);

    for (std::size_t i = 0; i < g.size(); ++i)
    {
        if (hasCycleHelp(g, i, visited, stack))
            return true;
    }

    return false;
}
