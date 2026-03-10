#include <bits/stdc++.h>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  int idx = -1;
  cin >> n;
  vector<int> arr(n);
  for (int i = 0; i < n; ++i)
    cin >> arr[i];
  vector<int> oddIdxs, evenIdxs;
  for (int i = 0; i < n; ++i) {
    if (arr[i] % 2 == 0)
      evenIdxs.push_back(i);
    else
      oddIdxs.push_back(i);
  }
  if (oddIdxs.size() == 1)
    idx = oddIdxs[0];
  else if (evenIdxs.size() == 1)
    idx = evenIdxs[0];
  cout << idx + 1 << '\n';
  return 0;
}