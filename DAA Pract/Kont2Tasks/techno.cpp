// #include <bits/stdc++.h>

// using namespace std;

// #define MAX 1010
// #define PA pair<int, int>

// int n, k, r, a, dist[MAX], total_cost = 0;

// vector<int> nodes[MAX];
// bool visited[MAX];

// int cost(int u, int v)
// {
//     int idx1 = 0, idx2 = 0;

//     while (idx1 < nodes[u].size() && idx2 < nodes[v].size())
//     {
//         if (nodes[u][idx1] == nodes[v][idx2])
//             return nodes[u][idx1];
//         else if (nodes[u][idx1] < nodes[v][idx2])
//             idx1++;
//         else
//             idx2++;
//     }

//     return k + 1;
// }

// void prim(int node)
// {
//     priority_queue<PA, vector<PA>, greater<PA>> pq;
//     fill(dist, dist + n + 1, INT_MAX);

//     dist[node] = 0;

//     for (int i = 0; i < n; i++)
//     {
//         int best = -1, currMin = INT_MAX;
//         for (int j = 1; j <= n; j++)
//         {
//             if (!visited[j] && currMin > dist[j])
//             {
//                 currMin = dist[j];
//                 best = j;
//             }
//         }
//         visited[best] = true;
//         total_cost += dist[best];

//         for (int i = 1; i <= n; i++)
//         {
//             if (!visited[i])
//             {
//                 int currCost = cost(best, i);
//                 if (currCost < dist[i])
//                     dist[i] = currCost;
//             }
//         }
//     }
// }

// int main()
// {
//     cin >> n >> k;

//     for (int i = 1; i <= n; i++)
//     {
//         cin >> r;

//         for (int j = 0; j < r; j++)
//         {
//             cin >> a;
//             nodes[i].push_back(a);
//         }
//     }

//     prim(1);

//     cout << total_cost << '\n';

//     return 0;
// }

#include <bits/stdc++.h>

using namespace std;

#define MAX 1100

int n, k, r, a, par[MAX], d[MAX];

vector<int> nodes[MAX];
vector<pair<int, pair<int, int>>> edges;

int find(int node)
{
    if (par[node] == 0)
        return node;

    return par[node] = find(par[node]);
}

void myUnion(int u, int v)
{
    u = find(u);
    v = find(v);

    if (u == v)
        return;

    if (d[u] > d[v])
        par[v] = u;
    else
    {
        par[u] = v;
        if (d[u] == d[v])
            d[v]++;
    }
}

int cost(int u, int v)
{
    int idx1 = 0, idx2 = 0;

    while (idx1 < nodes[u].size() && idx2 < nodes[v].size())
    {
        if (nodes[u][idx1] == nodes[v][idx2])
            return nodes[u][idx1];
        else if (nodes[u][idx1] < nodes[v][idx2])
            idx1++;
        else
            idx2++;
    }

    return k + 1;
}

int main()
{
    cin >> n >> k;

    for (int i = 1; i <= n; i++)
    {
        cin >> r;

        for (int j = 0; j < r; j++)
        {
            cin >> a;
            nodes[i].push_back(a);
        }
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            edges.push_back({cost(i, j), {i, j}});
        }
    }

    sort(edges.begin(), edges.end());

    int total_cost = 0;

    for (int i = 0; i < edges.size(); i++)
    {
        int u = edges[i].second.first;
        int v = edges[i].second.second;

        if (find(u) != find(v))
        {
            total_cost += edges[i].first;
            myUnion(u, v);
        }
    }

    cout << total_cost << '\n';

    return 0;
}