#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
長方形への一括加算と、各マスの値・長方形の合計を扱う2次元いもす法。
座標は0始まり、長方形は半開区間 [r1, r2) × [c1, c2)。

コンストラクタ:
- `Imos2D(h, w)`: h は高さ、w は幅（ともに0以上）。全要素0の盤面を作る。
- `Imos2D(initial)`: initial は長方形の初期値配列。空配列も指定できる。

関数:
- `add(r1, c1, r2, c2, value)`: 長方形に加算する。空区間も指定できる。
- `add(r, c, value)`: 1マスに加算する。
- `build()`: 保留中の加算を反映する。取得時にも自動で呼ばれる。
- `get(r, c)`: 1マスの値を返す。
- `sum(r1, c1, r2, c2)`: 長方形の合計を返す。
- `values()`: 番兵を含まない高さ h、幅 w の配列を返す。

add は O(1)。更新後の最初の取得・build は O(HW)、以後の get・sum は O(1)。
values は配列をコピーするため O(HW)。メモリ O(HW)。
更新をまとめてから取得すると効率がよい。値・累積和は long long の範囲内で使う。

使用例:
```cpp
koma::Imos2D grid(2, 3);
grid.add(0, 1, 2, 3, 4); // 行0〜1、列1〜2に4を加算
grid.add(1, 2, 3);       // (1, 2) にさらに3を加算
grid.get(1, 2);         // 7（累積処理は自動）
grid.sum(0, 0, 2, 3);   // 19
std::vector<std::vector<long long>> result = grid.values();
```
*/
class Imos2D {
	long long h_, w_;
	std::vector<std::vector<long long>> diff_;
	mutable std::vector<std::vector<long long>> prefix_;
	mutable bool dirty_ = true;

	void check_rect(long long r1, long long c1, long long r2, long long c2) const {
		assert(0 <= r1 && r1 <= r2 && r2 <= h_);
		assert(0 <= c1 && c1 <= c2 && c2 <= w_);
	}

  public:
	/**
	高さ h、幅 w の全要素0の盤面を作る。h、w は0以上。
	時間・メモリ O((h+1)(w+1))。
	使用例: `koma::Imos2D grid(3, 4);`
	*/
	Imos2D(long long h, long long w) : h_(h), w_(w) {
		assert(0 <= h && h < std::numeric_limits<long long>::max());
		assert(0 <= w && w < std::numeric_limits<long long>::max());
		diff_.assign(h + 1, std::vector<long long>(w + 1, 0));
	}

	/**
	長方形の初期値配列 initial から作る。空配列は高さ・幅ともに0とする。
	時間・メモリ O((H+1)(W+1))。
	使用例: `koma::Imos2D grid({{1, 2}, {3, 4}});`
	*/
	explicit Imos2D(const std::vector<std::vector<long long>> &initial)
		: Imos2D(static_cast<long long>(initial.size()),
				 initial.empty() ? 0 : static_cast<long long>(initial.front().size())) {
		for(const auto &row : initial) assert(row.size() == static_cast<std::size_t>(w_));
		for(long long r = 0; r < h_; ++r)
			for(long long c = 0; c < w_; ++c)
				add(r, c, initial[r][c]);
	}

	/** 高さを返す。O(1)。使用例: `grid.height();` */
	long long height() const { return h_; }

	/** 幅を返す。O(1)。使用例: `grid.width();` */
	long long width() const { return w_; }

	/**
	半開区間 [r1, r2) × [c1, c2) に value を加算する。空区間では何もしない。
	0 <= r1 <= r2 <= H、0 <= c1 <= c2 <= W。O(1)。
	使用例: `grid.add(0, 0, grid.height(), grid.width(), 5);`
	*/
	void add(long long r1, long long c1, long long r2, long long c2, long long value) {
		check_rect(r1, c1, r2, c2);
		if(r1 == r2 || c1 == c2 || value == 0) return;
		diff_[r1][c1] += value;
		diff_[r1][c2] -= value;
		diff_[r2][c1] -= value;
		diff_[r2][c2] += value;
		dirty_ = true;
	}

	/**
	マス (r, c) に value を加算する。0 <= r < H、0 <= c < W。O(1)。
	使用例: `grid.add(1, 2, 5);`
	*/
	void add(long long r, long long c, long long value) {
		assert(0 <= r && r < h_ && 0 <= c && c < w_);
		add(r, c, r + 1, c + 1, value);
	}

	/**
	保留中の加算を累積和に反映する。何度呼んでも結果は変わらない。
	更新後は O(HW)、更新がなければ O(1)。取得時にも自動実行する。
	使用例: `grid.build();`
	*/
	void build() const {
		if(!dirty_) return;
		auto cells = diff_;
		prefix_.assign(h_ + 1, std::vector<long long>(w_ + 1, 0));
		for(long long r = 0; r < h_; ++r) {
			for(long long c = 0; c < w_; ++c) {
				if(r > 0) cells[r][c] += cells[r - 1][c];
				if(c > 0) cells[r][c] += cells[r][c - 1];
				if(r > 0 && c > 0) cells[r][c] -= cells[r - 1][c - 1];
				prefix_[r + 1][c + 1] = cells[r][c] + prefix_[r][c + 1] + prefix_[r + 1][c] - prefix_[r][c];
			}
		}
		dirty_ = false;
	}

	/**
	マス (r, c) の値を返す。0 <= r < H、0 <= c < W。
	更新後の初回は O(HW)、以後 O(1)。使用例: `grid.get(1, 2);`
	*/
	long long get(long long r, long long c) const {
		assert(0 <= r && r < h_ && 0 <= c && c < w_);
		return sum(r, c, r + 1, c + 1);
	}

	/**
	半開区間 [r1, r2) × [c1, c2) の合計を返す。空区間なら0。
	0 <= r1 <= r2 <= H、0 <= c1 <= c2 <= W。
	更新後の初回は O(HW)、以後 O(1)。
	使用例: `grid.sum(0, 0, grid.height(), grid.width());`
	*/
	long long sum(long long r1, long long c1, long long r2, long long c2) const {
		check_rect(r1, c1, r2, c2);
		if(r1 == r2 || c1 == c2) return 0;
		build();
		return prefix_[r2][c2] - prefix_[r1][c2] - prefix_[r2][c1] + prefix_[r1][c1];
	}

	/**
	全マスの値を高さH、幅Wの配列として返す。番兵は含まない。
	時間・返す配列のメモリ O(HW)。使用例: `auto result = grid.values();`
	*/
	std::vector<std::vector<long long>> values() const {
		build();
		std::vector<std::vector<long long>> result(h_, std::vector<long long>(w_));
		for(long long r = 0; r < h_; ++r)
			for(long long c = 0; c < w_; ++c)
				result[r][c] = get(r, c);
		return result;
	}
};

} // namespace koma
