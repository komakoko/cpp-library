#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
大小関係を保ち、重複する値を同じ0始まりの添字に座標圧縮する。
T は大小比較可能で、比較が厳密弱順序を満たすこと。
構築は時間 O(N log(N+1))、メモリ O(N)。

コンストラクタ:
`CoordinateCompression()`: 空の圧縮表を作る。
`CoordinateCompression(a)`: 圧縮対象の `std::vector<T>` を登録する。元の配列は変更しない。

関数:
`index(x)`: 登録済みの値の添字。O(log(N+1))。未登録なら std::out_of_range。
`value(i)`: 添字に対応する元の値。O(1)。範囲外なら std::out_of_range。
`compress(a)`: 登録済みの値の配列を添字配列へ変換。長さ M に対して O(M log(N+1))。
`size()`: 異なる値の個数。O(1)。
`values()`: 重複を除いた昇順の値の配列へのconst参照。O(1)。

使用例:
```cpp
std::vector<long long> a{30, 10, 20, 20};
koma::CoordinateCompression cc(a);
auto ids = cc.compress(a); // {2, 0, 1, 1}
auto i = cc.index(20);     // 1
auto x = cc.value(1);      // 20
const auto& values = cc.values(); // {10, 20, 30}（重複を除いた昇順の元の値）
```
*/
template <class T>
class CoordinateCompression {
  public:
	/** 空の圧縮表を作る。 */
	CoordinateCompression() = default;

	/** a の値を登録する。時間 O(N log(N+1))、メモリ O(N)。 */
	explicit CoordinateCompression(std::vector<T> a) : values_(std::move(a)) {
		std::sort(values_.begin(), values_.end());
		values_.erase(std::unique(values_.begin(), values_.end(),
								  [](const T &a, const T &b) { return !(a < b) && !(b < a); }),
					  values_.end());
	}

	/** 登録済みの値 x の添字を返す。未登録なら std::out_of_range。O(log(N+1))。 */
	long long index(const T &x) const {
		auto it = std::lower_bound(values_.begin(), values_.end(), x);
		if(it == values_.end() || x < *it)
			throw std::out_of_range("CoordinateCompression: unregistered value");
		return it - values_.begin();
	}

	/** 添字 i に対応する元の値を返す。範囲外なら std::out_of_range。O(1)。 */
	T value(long long i) const { return values_.at(i); }

	/** 登録済みの値の配列を添字配列に変換する。長さ M に対して O(M log(N+1))。 */
	std::vector<long long> compress(const std::vector<T> &a) const {
		std::vector<long long> ids;
		ids.reserve(a.size());
		for(const auto &x : a) ids.push_back(index(x));
		return ids;
	}

	/** 異なる値の個数を返す。O(1)。 */
	long long size() const { return static_cast<long long>(values_.size()); }

	/** 重複を除いた昇順の値の配列をconst参照で返す。O(1)。 */
	const std::vector<T> &values() const { return values_; }

  private:
	std::vector<T> values_;
};

} // namespace koma
