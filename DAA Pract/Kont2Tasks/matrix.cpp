#include <bits/stdc++.h>

using namespace std;
#define MAX 1100

int n, m, k, dist[MAX][MAX], a, b;
char grid[MAX][MAX];

int mx[4] = {-1, 0, 0, 1}, my[4] = {0, -1, 1, 0};

int main()
{
    cin >> n >> m >> k;

    for (int i = 0; i < n; i++)
        fill(dist[i], dist[i] + m + 10, INT_MAX);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> grid[i][j];

    queue<pair<int, int>> q;

    for (int i = 0; i < k; i++)
    {
        cin >> a >> b;
        a = a - 1;
        b = b - 1;
        q.push({a, b});
        dist[a][b] = 0;
    }

    int curr = 0;
    while (!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        curr = dist[x][y];

        q.pop();

        for (int i = 0; i < 4; i++)
        {
            if (x + mx[i] < 0 && x + mx[i] >= n && y + my[i] < 0 && y + my[i] >= m)
                continue;

            if (grid[x + mx[i]][y + my[i]] == '.' && dist[x + mx[i]][y + my[i]] > curr + 1)
            {
                dist[x + mx[i]][y + my[i]] = curr + 1;
                q.push({x + mx[i], y + my[i]});
            }
        }
    }

    int total = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (dist[i][j] != INT_MAX)
                total += dist[i][j];

    cout << total << '\n';
}