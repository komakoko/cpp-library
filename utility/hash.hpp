#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
既存のハッシュ値に、値を1つ混ぜる。

引数:
`seed`: 更新するハッシュ値。最初は0で初期化する。
`value`: 混ぜる値。
`hasher`: 値のハッシュ関数。省略時は `std::hash<T>`。

返り値: なし。`seed` を更新する。
計算量: hasherの実行時間に加えて O(1)。追加する順序に依存する。
使用例:
```cpp
std::size_t seed = 0;
koma::hash_combine(seed, 42);
koma::hash_combine(seed, std::string("abc"));
```
*/
template <class T, class Hasher = std::hash<T>>
void hash_combine(std::size_t &seed, const T &value, Hasher hasher = {}) {
	seed ^= hasher(value) + static_cast<std::size_t>(0x9e3779b9U) + (seed << 6) + (seed >> 2);
}

/**
複数の値を引数の順に混ぜて、ハッシュ値を作る。

引数: `values`: 各型の `std::hash` が使える値。0個なら0を返す。
返り値: `std::size_t` のハッシュ値。
計算量: 各値のハッシュ計算量の合計に加えて O(引数の個数)。
独自の等価比較を使う場合、等しいキーのハッシュ値も等しくなるように値を選ぶ。
使用例:
```cpp
struct Point { int x, y; bool operator==(const Point&) const = default; };
struct PointHash {
	std::size_t operator()(const Point& p) const {
		return koma::hash_values(p.x, p.y);
	}
};
std::unordered_set<Point, PointHash> points;
```
*/
template <class... Ts>
std::size_t hash_values(const Ts &...values) {
	std::size_t seed = 0;
	(hash_combine(seed, values), ...);
	return seed;
}

} // namespace koma
