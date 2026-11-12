#include <bits/stdc++.h>

using namespace std;

int n, m;
const int maxn = 105;
bool a[maxn][maxn];
bool visited[maxn];

void dfs(int v)
{
    visited[v] = true;

    cout << v << ' ';

    for (int i = 0; i < n; i++)
    {
        if (a[v][i] && !visited[i])
            dfs(i);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    int u, v;
    for (int i = 1; i <= m; i++)
    {
        cin >> u >> v;
        a[u][v] = 1;
        a[v][u] = 1;
    }

    dfs(0);
    cout << endl;

    return 0;
}