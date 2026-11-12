#include <bits/stdc++.h>

using namespace std;

struct Point
{
    int x, flag, pos;

    bool operator<(const Point &other)
    {
        if (x != other.x)
            return x < other.x;

        return flag < other.flag;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, m;

    cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        
    }
}