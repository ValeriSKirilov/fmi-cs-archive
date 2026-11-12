#include <bits/stdc++.h>

using namespace std;

#define MAX 10005

int n, m, k;
vector<int> g[MAX];
bool visited[MAX];

int dx[4] = {0, 1, -1, 0};
int dy[4] = {-1, 0, 0, 1};

void dfs(int v)
{
    visited[v] = true;

    for (auto u : g[v])
    {
        if (!visited[u])
        {
            dfs(u);
        }
    }
}

int code(int i, int j)
{
    return (i - 1) * m + j;
}

bool valid(int x, int y)
{
    return 1 <= x && x <= n && 1 <= y && y <= m;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    int u, v;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            int u = code(i, j);

            int v;

            for (int t = 0; t < 4; t++)
            {
                if (valid(i + dx[t], j + dy[t]))
                {
                    v = code(i + dx[t], j + dy[t]);
                    g[u].push_back(v);
                    g[v].push_back(u);
                }
            }
        }
    }

    cin >> k;
    for (int i = 0; i < k; i++)
    {
        int x, y;
        cin >> x >> y;
        int u = code(x, y);
        visited[u] = true;
    }

    int count = 0;
    for (int i = 1; i <= n * m; i++)
    {
        if (!visited[i])
        {
            count++;
            dfs(i);
        }
    }

    cout << count << endl;

    return 0;
}