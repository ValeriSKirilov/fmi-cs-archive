#include <bits/stdc++.h>

using namespace std;

struct Event
{
    int x;
    int y1, y2;
    int type;

    bool operator<(const Event &other) const
    {
        return x < other.x;
    }
};

int n;
vector<Event> events;
int active_y[20005] = {0};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        int x1, y1, x2, y2;

        cin >> x1 >> y1 >> x2 >> y2;

        int leftX = min(x1, x2), rightX = max(x1, x2), bottomY = min(y1, y2), topY = max(y1, y2);

        if (leftX == rightX)
            continue;

        events.push_back({leftX, bottomY, topY, 1});

        events.push_back({rightX, bottomY, topY, -1});
    }

    sort(events.begin(), events.end());

    long long ans = 0, active_length = 0;

    if (events.empty())
    {
        cout << 0 << '\n';
        return 0;
    }
    int prev_x = events[0].x;

    for (int i = 0; i < events.size(); i++)
    {
        Event current_event = events[i];

        long long distance = current_event.x - prev_x;
        ans += distance * active_length;

        for (int j = current_event.y1 + 10000; j < current_event.y2 + 10000; j++)
            active_y[j] += current_event.type;

        active_length = 0;
        for (int j = 0; j <= 20000; j++)
        {
            if (active_y[j] > 0)
                active_length++;
        }

        prev_x = current_event.x;
    }

    cout << ans << '\n';

    return 0;
}