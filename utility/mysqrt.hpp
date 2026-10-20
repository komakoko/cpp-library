#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
非負整数 n の k 乗根を切り捨てた整数を返す。k を省略すると平方根を返す。
引数:
`n`: 0以上の整数。
`k`: 1以上の整数の次数。既定値は2。

返り値:
long long型の最大の整数 r（r^k <= n）。浮動小数点の丸め誤差は生じない。

計算量:
n のビット数を B として、k >= B なら O(1)、それ以外は O(ceil(B/k) * log(k+1))。

使用例:
```cpp
koma::mysqrt(10);    // 3
koma::mysqrt(30, 3); // 3
koma::mysqrt(16, 4); // 2
```
*/
inline long long mysqrt(long long n, int k = 2) {
	assert(n >= 0);
	assert(k >= 1);
	if(k == 1 || n <= 1) return n;
	int bits = std::bit_width(static_cast<unsigned long long>(n));
	if(k >= bits) return 1;

	auto fits = [n, k](long long x) {
		if(k == 2) return static_cast<__int128>(x) * x <= n;
		__int128 value = 1, base = x;
		int exponent = k;
		while(exponent > 0) {
			if(exponent & 1) {
				value *= base;
				if(value > n) return false;
			}
			exponent >>= 1;
			if(exponent) {
				base *= base;
				if(base > n) return false;
			}
		}
		return true;
	};
	long long ok = 1, ng = 1LL << ((bits + k - 1) / k);
	while(ng - ok > 1) {
		long long mid = (ok + ng) / 2;
		if(fits(mid)) ok = mid;
		else ng = mid;
	}
	return ok;
}

} // namespace koma
