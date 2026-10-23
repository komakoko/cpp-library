#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
繰り返し二乗法で base の k 乗を返す。
引数: `base`: 底。`k`: 0以上の整数の指数。
返り値: T型の累乗。k=0では1（0の0乗も1）。演算結果がTの範囲に収まること。
計算量: 乗算を O(1) として O(log(k+1))。
使用例: `koma::ipow(2LL, 10); // 1024`
*/
template <class T> T ipow(T base, long long k) {
	assert(k >= 0); T result = 1;
	while(k > 0) {
		if(k & 1) result *= base;
		k >>= 1;
		if(k) base *= base;
	}
	return result;
}

} // namespace koma
