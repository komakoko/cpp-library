#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
非負整数の文字列を、指定した基数に変換する。整数型に収まらない桁数にも対応する。

引数:
`number`: 変換する整数の文字列。数字は0〜9、英字はA〜Z・a〜zを使用する。
`from`: 変換前の基数（2〜36）。
`to`: 変換後の基数（2〜36）。

返り値:
変換後の `std::string`。英字は大文字、先頭の0は除く。値が0なら `"0"`。
空文字・不正な数字・範囲外の基数では `std::invalid_argument` が発生する。
符号・小数点・空白は受け付けない。

計算量:
入力桁数を N として時間 O(N²)、メモリ O(N)。

使用例:
```cpp
koma::radix("1010", 2, 10); // "10"
koma::radix("255", 10, 16); // "FF"
koma::radix("ff", 16, 10);  // "255"
koma::radix("000", 10, 2);  // "0"
```
*/
inline std::string radix(const std::string &number, long long from, long long to) {
	if(from < 2 || from > 36 || to < 2 || to > 36 || number.empty())
		throw std::invalid_argument("radix: expected bases 2..36 and a nonempty number");
	std::vector<int> digits;
	for(char c : number) {
		int value = c >= '0' && c <= '9'   ? c - '0'
					: c >= 'A' && c <= 'Z' ? c - 'A' + 10
					: c >= 'a' && c <= 'z' ? c - 'a' + 10
										   : -1;
		if(value < 0 || value >= from) throw std::invalid_argument("radix: invalid digit");
		if(value || !digits.empty()) digits.push_back(value);
	}
	if(digits.empty()) return "0";
	constexpr char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string result;
	while(!digits.empty()) {
		std::vector<int> quotient;
		int remainder = 0;
		for(int digit : digits) {
			int value = remainder * (int)from + digit;
			int q = value / (int)to;
			remainder = value % (int)to;
			if(q || !quotient.empty()) quotient.push_back(q);
		}
		result += alphabet[remainder];
		digits = std::move(quotient);
	}
	std::reverse(result.begin(), result.end());
	return result;
}

} // namespace koma
