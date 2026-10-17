// https://github.com/komakoko/cpp-library
#include <atcoder/all>
#include <bits/stdc++.h>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef std::pair<ll, ll> pll;
#define sz(a) static_cast<ll>(a.size())
#define rep(i, n) for(ll i = 0; i < (ll)(n); ++i)
#define rep1(i, n) for(ll i = 1; i <= (ll)(n); ++i)
#define rrep(i, n) for(ll i = (ll)(n) - 1; i >= 0; --i)
#define rrep1(i, n) for(ll i = (ll)(n); i >= 1; --i)
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#include <koma/common>
// #include <koma/graph/all>
using namespace koma;

void solve() {
	string s;
	cin >> s;
	for(char c : s) {
		if(c != 'A') cout << '.';
		else cout << 'A';
	}
	cout << endl;
	return;
}

int main() {
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	int test_case = 1;
	// cin >> test_case;
	cout << std::setprecision(15);
	while(test_case--) {
		solve();
	}
	return 0;
}
