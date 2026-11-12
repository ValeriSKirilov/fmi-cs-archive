#include <bits/stdc++.h>

using namespace std;

#define MAX 11000

int n, m, d[MAX];

vector<int> g[MAX], ans;

priority_queue<int, vector<int>, greater<int> > pq;

void topo()
{
    for (int i = 1; i <= n; i++)
        if (d[i] == 0)
            pq.push(i);

    while (!pq.empty())
    {
        int node = pq.top();
        pq.pop();

        ans.push_back(node);
        for (auto u : g[node])
        {
            d[u]--;
            if (d[u] == 0)
                pq.push(u);
        }
    }

    if (ans.size() == n)
    {
        for (auto x : ans)
            cout << x << " ";
        cout << endl;
    }
    else
    {
        cout << "Sandro fails." << endl;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    int u, v;
    for (int i = 0; i < m; i++)
    {
        cin >> u >> v;
        g[u].push_back(v);
        d[v]++;
    }

    topo();

    return 0;
}