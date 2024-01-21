#ifdef LOCAL
    # define _GLIBCXX_DEBUG
#endif
#include <bits/stdc++.h>
#include <immintrin.h>

#ifndef LOCAL
    // #pragma GCC optimize("O3")
    // #pragma GCC optimize("Ofast")
    // #pragma GCC optimize("unroll-loops")
    // #pragma GCC target("avx,avx2,bmi,bmi2,popcnt,lzcnt,sse,sse2,sse3,ssse3,mmx")
#endif
using namespace std;
using ll = long long;
using ld = long double;
# define x first
# define y second
# define all(x) x.begin(), x.end()
# define rall(x) x.rbegin(), x.rend()

mt19937_64 mt(time(0));

void solve();
void init();

int32_t main() {
#ifndef LOCAL
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
    cout << fixed << setprecision(30);
    init();
    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
}

void init() {}

void solve() {
    int n;
    cin >> n;
    cout << n;
}
