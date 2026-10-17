#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
負の被除数にも対応し、0以上 b 未満の剰余を返す。
引数: `a`: 被除数。`b`: 正の除数。
返り値: long long型の非負の剰余。
計算量: O(1)。
使用例: `koma::mymod(-1, 5); // 4`
*/
inline long long mymod(long long a, long long b) {
	assert(b > 0);
	long long r = a % b;
	return r < 0 ? r + b : r;
}

} // namespace koma
