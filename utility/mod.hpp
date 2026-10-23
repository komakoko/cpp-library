#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
負の被除数にも対応し、0以上 b 未満の剰余を返す。
使用例: `koma::mod(-1, 5); // 4`
*/
inline long long mod(long long a, long long b) {
	assert(b > 0);
	long long r = a % b;
	return r < 0 ? r + b : r;
}

} // namespace koma
