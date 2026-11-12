#include <bits/stdc++.h>

using namespace std;

#define MAX 110000

int n;

vector<int> g[MAX];
bool visited[MAX];

vector<int> curr_ring;

vector<int> answer;

void dfs(int node)
{
    if (visited[node])
        return;

    visited[node] = true;

    curr_ring.push_back(node);

    if (!visited[g[node][0]])
        dfs(g[node][0]);

    if (!visited[g[node][1]])
        dfs(g[node][1]);
}

void solve(int curr)
{
    curr_ring.clear();

    dfs(curr);

    vector<int> temp1, temp2;

    if (curr_ring.size() % 2 == 0)
    {
        for (int i = 0; i < curr_ring.size(); i += 2)
            temp1.push_back(curr_ring[i]);

        for (int i = 1; i < curr_ring.size(); i += 2)
            temp2.push_back(curr_ring[i]);
    }
    else
    {
        answer.push_back(curr_ring[0]);

        for (int i = 2; i < curr_ring.size() - 1; i += 2)
            temp1.push_back(curr_ring[i]);

        for (int i = 3; i < curr_ring.size() - 1; i += 2)
            temp2.push_back(curr_ring[i]);
    }

    sort(temp1.begin(), temp1.end());
    sort(temp2.begin(), temp2.end());

    if (temp1 < temp2)
    {
        answer.insert(answer.end(), temp1.begin(), temp1.end());
    }
    else
    {
        answer.insert(answer.end(), temp2.begin(), temp2.end());
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n;

    int u, v;
    for (int i = 0; i < n; i++)
    {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    for (int i = 1; i <= n; i++)
    {
        if (visited[i] == false)
        {
            solve(i);
        }
    }

    sort(answer.begin(), answer.end());

    cout << answer.size() << '\n';
    for (int i = 0; i < answer.size(); i++)
    {
        cout << answer[i] << ' ';
    }
    cout << "\n";

    return 0;
}