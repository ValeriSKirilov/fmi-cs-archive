#include <bits/stdc++.h>

using namespace std;

int k;
string s;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> k >> s;

  int curr_count = 0, ans = 0, last_diff_streak = 0, best_start = 1;
  char last_letter = '*';
  bool found_any_tl = false;

  for (int i = 0; i < s.length(); i++) {
    if (s[i] - 'a' + 1 <= k) {
      if (!found_any_tl) {
        found_any_tl = true;
        last_letter = s[i];
        curr_count = last_diff_streak + 1;
        last_diff_streak = 0;
      } else {
        if (s[i] == last_letter) {
          curr_count++;
          last_diff_streak = 0;
        } else {
          curr_count = last_diff_streak + 1;
          last_letter = s[i];
          last_diff_streak = 0;
        }
      }
    } else {
      last_diff_streak++;
      if (found_any_tl) curr_count++;
    }

    if (found_any_tl && curr_count > ans) {
      ans = curr_count;
      best_start = i - curr_count + 1;
    }
  }

  if (found_any_tl) ans = max(curr_count, ans);

  cout << ans << '\n';
  cout << s.substr(best_start, ans) << '\n';

  return 0;
}