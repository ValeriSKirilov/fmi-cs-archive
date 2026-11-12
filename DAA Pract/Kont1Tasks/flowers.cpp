#include <bits/stdc++.h>

using namespace std;

#define MAX 105

int n, k;

vector<long long> flowers(MAX);

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> k;

    for (int i = 0; i < n; i++)
        cin >> flowers[i];

    sort(flowers.begin(), flowers.begin() + n);
    reverse(flowers.begin(), flowers.begin() + n);

    long long answer = 0;
    int currMult = 1, counter = k;

    for (int i = 0; i < n; i++)
    {
        if (counter > 0)
        {
            answer += currMult * flowers[i];
            counter--;
        }
        else
        {
            counter = k - 1;
            currMult++;
            answer += currMult * flowers[i];
        }
    }

    cout << answer << '\n';

    return 0;
}