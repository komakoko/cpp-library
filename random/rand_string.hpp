#pragma once
#include "rand_ll.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
指定した文字からランダムな文字列を生成する。

引数:
`n`: 文字列の長さ。0以上。
`alphabet`: 使用する文字。既定値は英小文字26文字。n > 0なら空にできない。
各位置でalphabetから1文字を選ぶ。同じ文字を複数含めると、その文字が選ばれやすくなる。
共通の生成器 `rand_engine` を使う。不正な引数では `std::invalid_argument`。

返り値:
長さnの `std::string`。n = 0なら空文字列。

計算量:
期待 O(n)。

使用例:
```cpp
auto s = koma::rand_string(10);        // 英小文字10文字
auto t = koma::rand_string(5, "01"); // 0と1からなる5文字
```
*/
inline std::string rand_string(long long n, std::string_view alphabet = "abcdefghijklmnopqrstuvwxyz") {
	if(n < 0 || (n > 0 && alphabet.empty()))
		throw std::invalid_argument("rand_string: expected n >= 0 and nonempty alphabet when n > 0");
	std::string result(static_cast<std::size_t>(n), '\0');
	for(auto &c : result) c = alphabet[rand_ll(0, static_cast<long long>(alphabet.size()) - 1)];
	return result;
}

} // namespace koma
