#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> points(n);
    vector<bool> line(10002, false);

    int highest = 0;
    for (int i = 0; i < n; i++)
    {
        cin >> points[i].first >> points[i].second;
        highest = max(highest, points[i].second);
        for (int j = points[i].first - 1; j < points[i].second - 1; j++)
            line[j] = 1;
    }

    int totalLines = 0, maxLen = 0;
    int i = 0;
    while (i < highest)
    {
        int currLen = 0;
        if (line[i])
        {
            int idx = i;
            while (line[idx])
            {
                currLen++;
                idx++;
            }
            maxLen = max(maxLen, currLen);
            i = idx + 1;
            totalLines++;
        }
        else
            i++;
    }

    cout << totalLines << ' ' << maxLen;
}