#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace koma {

/**
重複を許す順序付き集合で、順位からの要素取得と、指定値より前にある要素数の取得ができる。
GNU PBDS が必要。T の既定は long long、Compare の既定は std::less<T>。
Compare は厳密弱順序を満たすこと。等価な値も別々の要素として順位に数える。
イテレータは値へのconst参照を返す。挿入では既存のイテレータを無効化しない。

コンストラクタ:
`ordered_multi_set<T, Compare>()`: 空の集合を作る。
`ordered_multi_set<T, Compare>(comp)`: 比較関数 comp を指定して空の集合を作る。
`ordered_multi_set<T, Compare>({a, b, ...}, comp = Compare())`: 初期値を登録する。O(n log(n+1))。
`ordered_multi_set<T, Compare>(first, last, comp = Compare())`: 範囲 [first, last) を登録する。O(n log(n+1))。

関数:
`insert(x)`: x を1個追加し、そのイテレータを返す。O(log(n+1))。
`erase(x)`: x と等価な要素をすべて削除し、削除数を返す。削除数を m として O((m+1) log(n+1))。
`erase(it)`: it の1個を削除し、次のイテレータを返す。it は end() 以外。O(log(n+1))。
`erase_one(x)`: x と等価な要素を1個削除し、削除できたかを返す。O(log(n+1))。
`find_by_order(k)`: 0始まりで k 番目のイテレータ。k >= size() なら end()。O(log(n+1))。
`order_of_key(x)`: Compare の順で x より前にある要素の個数。O(log(n+1))。
`find(x)` / `contains(x)` / `count(x)`: 等価な要素の検索・存在判定・個数。O(log(n+1))。
`lower_bound(x)` / `upper_bound(x)` / `equal_range(x)`: std::multiset と同じ境界検索。O(log(n+1))。
`begin()` / `end()` / `cbegin()` / `cend()`: 双方向イテレータ。O(1)。
`rbegin()` / `rend()` / `crbegin()` / `crend()`: 逆順イテレータ。O(1)。
`size()` / `empty()`: 要素数・空判定。O(1)。`clear()`: 全削除。O(n)。
`key_comp()`: 比較関数のコピーを返す。O(1)。

使用例:
```cpp
koma::ordered_multi_set<> s{5, 2, 5};
s.insert(3);
auto x = *s.find_by_order(2); // 5（{2, 3, 5, 5} の2番目）
auto k = s.order_of_key(5);  // 2
s.erase_one(5);             // {2, 3, 5}
s.erase(s.find_by_order(0)); // {3, 5}
auto n = s.erase(5);        // 1、残りは {3}
```
*/
template <class T = long long, class Compare = std::less<T>>
class ordered_multi_set {
  private:
	using id_type = std::uint64_t;
	/** 値と、重複を区別する挿入IDを保持する。 */
	struct entry {
		T first;
		id_type second;
		/** PBDSのデバッグ診断用に挿入IDと、出力可能なら値を表示する。 */
		friend std::ostream &operator<<(std::ostream &out, const entry &x) {
			out << "id=" << x.second;
			if constexpr(requires { out << x.first; }) out << ", value=" << x.first;
			return out;
		}
	};
	/** 値を比較し、等価な値は挿入順に並べる。 */
	struct entry_compare {
		Compare comp;
		/** 2つの要素の順序を比較する。 */
		bool operator()(const entry &a, const entry &b) const {
			if(comp(a.first, b.first)) return true;
			if(comp(b.first, a.first)) return false;
			return a.second < b.second;
		}
	};
	using tree_type = __gnu_pbds::tree<entry, __gnu_pbds::null_type, entry_compare,
									   __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
	tree_type tree_;
	id_type next_id_ = 1;
	static constexpr id_type upper_id_ = std::numeric_limits<id_type>::max();

  public:
	using value_type = T;
	using key_type = T;
	using key_compare = Compare;
	using size_type = typename tree_type::size_type;
	using difference_type = std::ptrdiff_t;

	/** 集合の値を変更せずに前後へ走査する双方向イテレータ。 */
	class const_iterator {
		friend class ordered_multi_set;
		using base_iterator = typename tree_type::const_iterator;
		base_iterator it_;
		/** 内部イテレータから構築する。 */
		explicit const_iterator(base_iterator it) : it_(it) {}

	  public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T *;
		using reference = const T &;
		/** 未関連のイテレータを作る。 */
		const_iterator() = default;
		/** 現在の値へのconst参照を返す。 */
		reference operator*() const { return it_->first; }
		/** 現在の値へのconstポインタを返す。 */
		pointer operator->() const { return std::addressof(it_->first); }
		/** 次の要素へ進む。 */
		const_iterator &operator++() {
			++it_;
			return *this;
		}
		/** 次の要素へ進み、移動前の位置を返す。 */
		const_iterator operator++(int) {
			auto old = *this;
			++*this;
			return old;
		}
		/** 前の要素へ戻る。 */
		const_iterator &operator--() {
			--it_;
			return *this;
		}
		/** 前の要素へ戻り、移動前の位置を返す。 */
		const_iterator operator--(int) {
			auto old = *this;
			--*this;
			return old;
		}
		/** 同じ位置を指すかを返す。 */
		bool operator==(const const_iterator &other) const { return it_ == other.it_; }
	};
	using iterator = const_iterator;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using reverse_iterator = const_reverse_iterator;

	/** 空の集合を作る。 */
	ordered_multi_set() : ordered_multi_set(Compare()) {}
	/** 比較関数 comp を指定して空の集合を作る。 */
	explicit ordered_multi_set(const Compare &comp) : tree_(entry_compare{comp}) {}
	/** 初期値を登録する。O(n log(n+1))。 */
	ordered_multi_set(std::initializer_list<T> init, const Compare &comp = Compare())
		: ordered_multi_set(init.begin(), init.end(), comp) {}
	/** 範囲 [first, last) を登録する。O(n log(n+1))。 */
	template <class InputIt>
	ordered_multi_set(InputIt first, InputIt last, const Compare &comp = Compare())
		: ordered_multi_set(comp) {
		for(; first != last; ++first) insert(*first);
	}

	/** 要素数を返す。O(1)。 */
	size_type size() const { return tree_.size(); }
	/** 集合が空かを返す。O(1)。 */
	bool empty() const { return tree_.empty(); }
	/** 比較関数のコピーを返す。O(1)。 */
	Compare key_comp() const { return tree_.get_cmp_fn().comp; }
	/** 先頭のイテレータを返す。O(1)。 */
	iterator begin() const { return iterator(tree_.begin()); }
	/** 末尾の次のイテレータを返す。O(1)。 */
	iterator end() const { return iterator(tree_.end()); }
	/** 先頭のconstイテレータを返す。O(1)。 */
	const_iterator cbegin() const { return begin(); }
	/** 末尾の次のconstイテレータを返す。O(1)。 */
	const_iterator cend() const { return end(); }
	/** 逆順の先頭イテレータを返す。O(1)。 */
	reverse_iterator rbegin() const { return reverse_iterator(end()); }
	/** 逆順の末尾の次のイテレータを返す。O(1)。 */
	reverse_iterator rend() const { return reverse_iterator(begin()); }
	/** 逆順の先頭constイテレータを返す。O(1)。 */
	const_reverse_iterator crbegin() const { return rbegin(); }
	/** 逆順の末尾の次のconstイテレータを返す。O(1)。 */
	const_reverse_iterator crend() const { return rend(); }

	/** x を1個追加し、そのイテレータを返す。O(log(n+1))。挿入IDを使い切ると std::length_error。 */
	iterator insert(const T &x) {
		if(next_id_ == upper_id_) throw std::length_error("ordered_multi_set: insertion ID exhausted");
		auto result = tree_.insert({x, next_id_});
		++next_id_;
		return iterator(result.first);
	}
	/** x と等価な要素をすべて削除し、削除数を返す。O((削除数+1) log(n+1))。 */
	size_type erase(const T &x) {
		auto [first, last] = equal_range(x);
		size_type removed = 0;
		while(first != last) {
			first = erase(first);
			++removed;
		}
		return removed;
	}
	/** it の1個を削除し、次のイテレータを返す。it は end() 以外。O(log(n+1))。 */
	iterator erase(iterator it) {
		auto next = it;
		++next;
		tree_.erase(it.it_);
		return next;
	}
	/** x と等価な要素を1個削除し、削除できたかを返す。O(log(n+1))。 */
	bool erase_one(const T &x) {
		auto it = find(x);
		if(it == end()) return false;
		erase(it);
		return true;
	}
	/** 全要素を削除する。O(n)。 */
	void clear() {
		tree_.clear();
		next_id_ = 1;
	}
	/** 0始まりで k 番目のイテレータを返す。範囲外なら end()。O(log(n+1))。 */
	iterator find_by_order(size_type k) const { return iterator(tree_.find_by_order(k)); }
	/** Compare の順で x より前にある要素数を返す。O(log(n+1))。 */
	size_type order_of_key(const T &x) const { return tree_.order_of_key({x, 0}); }
	/** x より前でない最初の要素を返す。なければ end()。O(log(n+1))。 */
	iterator lower_bound(const T &x) const { return iterator(tree_.lower_bound({x, 0})); }
	/** x より後にある最初の要素を返す。なければ end()。O(log(n+1))。 */
	iterator upper_bound(const T &x) const { return iterator(tree_.lower_bound({x, upper_id_})); }
	/** x と等価な要素の範囲 [first, last) を返す。O(log(n+1))。 */
	std::pair<iterator, iterator> equal_range(const T &x) const { return {lower_bound(x), upper_bound(x)}; }
	/** x と等価な最初の要素を返す。なければ end()。O(log(n+1))。 */
	iterator find(const T &x) const {
		auto it = lower_bound(x);
		if(it == end() || tree_.get_cmp_fn().comp(x, *it)) return end();
		return it;
	}
	/** x と等価な要素が存在するかを返す。O(log(n+1))。 */
	bool contains(const T &x) const { return find(x) != end(); }
	/** x と等価な要素の個数を返す。O(log(n+1))。 */
	size_type count(const T &x) const { return tree_.order_of_key({x, upper_id_}) - order_of_key(x); }
};

} // namespace koma
