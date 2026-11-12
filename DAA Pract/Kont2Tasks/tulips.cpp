#include <bits/stdc++.h>

using namespace std;

#define MAX 1100

int n, m, ccount = 0;

int garden[MAX][MAX];
bool visited[MAX][MAX];

void dfs(int x, int y, int curr_val)
{
    if (x < 1 || x > n || y < 1 || y > m)
        return;

    if (visited[x][y] || garden[x][y] != curr_val)
        return;

    visited[x][y] = true;
    dfs(x - 1, y, curr_val);
    dfs(x + 1, y, curr_val);
    dfs(x, y - 1, curr_val);
    dfs(x, y + 1, curr_val);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cin >> garden[i][j];
        }
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (!visited[i][j])
            {
                dfs(i, j, garden[i][j]);
                ccount++;
            }
        }
    }

    cout << ccount << '\n';
}