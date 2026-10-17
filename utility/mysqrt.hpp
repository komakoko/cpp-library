#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
非負整数 n の平方根を切り捨てた整数を返す。
引数: `n`: 0以上の整数。
返り値: long long型の整数平方根。浮動小数点の丸め誤差は生じない。
計算量: O(log(n+1))。
使用例: `koma::mysqrt(10); // 3`
*/
inline long long mysqrt(long long n) {
	assert(n >= 0);
	long long ok = 0, ng = 3037000500LL;
	while(ng - ok > 1) {
		long long mid = (ok + ng) / 2;
		if(mid <= n / mid) ok = mid;
		else ng = mid;
	}
	return ok;
}

} // namespace koma
