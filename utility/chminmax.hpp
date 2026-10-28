#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
a より b が小さい場合に a を更新する。
引数: `a`: 更新対象。`b`: 比較する値。
返り値: 更新した場合true、そうでなければfalse。
*/
template <class T>
bool chmin(T &a, const T &b) {
	if(a > b) {
		a = b;
		return true;
	}
	return false;
}

/**
a より b が大きい場合に a を更新する。
引数: `a`: 更新対象。`b`: 比較する値。
返り値: 更新した場合true、そうでなければfalse。
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
