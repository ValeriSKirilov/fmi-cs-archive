#include <bits/stdc++.h>

using namespace std;

#define MAX 110000

int n, q, m, u, v, in[MAX], depths[MAX];

vector<int> gr[MAX];
bool visited[MAX];

int dfs_modified(int node)
{
    visited[node] = true;

    int total_subordinates = 0;

    for (auto x : gr[node])
    {
        if (!visited[x])
            total_subordinates += dfs_modified(x) + 1;
    }

    return depths[node] = total_subordinates;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n;

    for (int i = 0; i < n - 1; i++)
    {
        cin >> u >> v;
        gr[u].push_back(v);
        in[v]++;
    }

    int root = -1;

    for (int i = 1; i <= n; i++)
        if (in[i] == 0)
            root = i;

    dfs_modified(root);

    cin >> q;

    for (int i = 0; i < q; i++)
    {
        cin >> m;
        cout << depths[m] << '\n';
    }

    return 0;
}