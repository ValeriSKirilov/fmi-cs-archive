//
// Created by Lenovo on 27.1.2026 г..
//

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

struct Graph
{
    std::vector<std::list<int>> adj;

    Graph(const int V) : adj(V)
    {
    };

    void addEdge(const int from, const int to)
    {
        adj[from].push_back(to);
        adj[to].push_back(from);
    }
};

struct ConComp
{
    std::vector<int> vertices;

    bool operator<(const ConComp& other) const
    {
        return vertices.size() < other.vertices.size();
    }
};

void dfs(const Graph& g, const int currVertex, std::vector<bool>& visited, ConComp& conComp)
{
    if (visited[currVertex])
        return;

    visited[currVertex] = true;
    conComp.vertices.push_back(currVertex);

    for (const int neigh : g.adj[currVertex])
    {
        dfs(g, neigh, visited, conComp);
    }
}

void getCompInfo(const Graph& g)
{
    std::vector<ConComp> conComps;
    std::vector visited(g.adj.size(), false);

    for (std::size_t i = 0; i < g.adj.size(); ++i)
    {
        if (!visited[i])
        {
            ConComp conComp;
            dfs(g, i, visited, conComp);
            conComps.push_back(conComp);
        }
    }

    std::cout << conComps.size() << std::endl;
    std::sort(conComps.begin(), conComps.end());

    for (const ConComp& cc : conComps)
    {
        for (const int vertex : cc.vertices)
        {
            std::cout << vertex << " ";
        }

        std::cout << std::endl;
    }
}
