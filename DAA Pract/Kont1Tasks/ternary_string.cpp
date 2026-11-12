#include <bits/stdc++.h>

using namespace std;

#define MAX 20005

int t;
vector<string> s(MAX);

int findSubstr(string curr)
{
    vector<int> counts(5, 0);
    int l = 0, ans = 0;

    for (int r = 0; r < curr.size(); r++)
    {
        counts[curr[r] - '0']++;

        while (counts[1] > 0 && counts[2] > 0 && counts[3] > 0)
        {
            int curr_len = r - l + 1;

            if (ans == 0)
                ans = curr_len;
            else
                ans = min(ans, curr_len);

            counts[curr[l] - '0']--;
            l++;
        }
    }

    return ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> t;

    for (int i = 0; i < t; i++)
        cin >> s[i];

    for (int i = 0; i < t; i++)
        cout << findSubstr(s[i]) << '\n';

    return 0;
}