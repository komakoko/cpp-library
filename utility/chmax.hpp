#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
a より b が大きい場合に a を更新する。
引数: `a`: 更新対象。`b`: 比較する値。同じ型で比較・代入できること。
返り値: 更新した場合true、そうでなければfalse。
計算量: 比較・代入を O(1) として O(1)。
使用例: `long long x = 3; koma::chmax(x, 10LL); // x=10、true`
*/
template <class T>
bool chmax(T &a, const T &b) {
	if(a < b) {
		a = b;
		return true;
	}
	return false;
}

} // namespace koma
