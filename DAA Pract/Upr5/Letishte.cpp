#include <bits/stdc++.h>

using namespace std;

struct Event
{
    int time, type, nationality;

    bool operator<(const Event &other) const
    {
        if (time != other.time)
            return time < other.time;

        return type > other.type;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n;
    cin >> n;

    vector<Event> allEvents(2 * n);

    for (int i = 0; i < n; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;

        allEvents[i] = {a, 1, c};
        allEvents[i + n] = {b, -1, c};
    }

    sort(allEvents.begin(), allEvents.end());

    int maxPassengers = 0;

    vector<int> currentCounts(3005, 0);
    for (const Event &e : allEvents)
    {
        currentCounts[e.nationality] += e.type;

        if (currentCounts[e.nationality] > maxPassengers)
            maxPassengers = currentCounts[e.nationality];
    }

    cout << maxPassengers << endl;
}