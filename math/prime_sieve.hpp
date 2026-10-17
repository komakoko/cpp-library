#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
素数一覧と、篩の範囲内の素数判定表を保持する。
`primes`: 昇順の素数一覧。
`is_prime`: 長さ n+1 の `std::vector<bool>`。`is_prime[x]` が true なら x は素数。
添字は0以上、篩の上限以下を指定する。判定の取得は O(1)。

使用例:
```cpp
auto result = koma::sieve_eratosthenes(20);
auto primes = result.primes; // {2, 3, 5, 7, 11, 13, 17, 19}
result.is_prime[17]; // true
result.is_prime[1];  // false
```
*/
struct SieveResult {
	std::vector<long long> primes;
	std::vector<bool> is_prime;
};

/**
エラストテネスの篩で、n以下の素数一覧と素数判定表を作る。
引数:
`n`: 篩の上限。0以上の整数。

返り値:
`SieveResult`。`primes` に素数一覧、`is_prime[x]` で0以上n以下の素数判定。
0と1は素数ではない。nが0または1の場合、素数一覧は空配列。

計算量:
時間 O(n log log n)、メモリ O(n)。

使用例:
```cpp
auto result = koma::sieve_eratosthenes(10);
auto primes = result.primes; // {2, 3, 5, 7}
bool prime = result.is_prime[7]; // true
```
*/
inline SieveResult sieve_eratosthenes(long long n) {
	assert(n >= 0);
	SieveResult result;
	result.is_prime.assign(n + 1, true);
	result.is_prime[0] = false;
	if(n >= 1) result.is_prime[1] = false;
	for(long long i = 2; i <= n; ++i) {
		if(!result.is_prime[i]) continue;
		result.primes.push_back(i);
		if(i <= n / i) {
			for(long long j = i * i; j <= n; j += i) result.is_prime[j] = false;
		}
	}
	return result;
}

// 素因数の数を全て求める
inline std::vector<long long> sieve_primefactor(long long n) {
	// O(NlogN)で1からnまでの素因数の数を全て求める
	assert(n >= 0);
	std::vector<long long> factor_count(n + 1, 0);
	for(long long i = 2; i <= n; i++) {
		if(factor_count[i] == 0) {
			for(long long j = i; j <= n; j += i) factor_count[j]++;
		}
	}
	return factor_count;
	// factor_count[i] は iの素因数の数
}

} // namespace koma
