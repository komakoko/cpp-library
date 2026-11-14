#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace koma {

/**
重複のない順序付き集合で、順位からの要素取得と、指定値より前にある要素数の取得ができる。
GNU PBDS が必要。T の既定は long long、Compare の既定は std::less<T>。
Compare は厳密弱順序を満たすこと。等価な値を再挿入しても要素数は増えない。
GNU PBDS の tree の操作をそのまま使える。

コンストラクタ:
`ordered_set<T, Compare>()`: 空の集合を作る。
`ordered_set<T, Compare>(comp)`: 比較関数 comp を指定して空の集合を作る。
`ordered_set<T, Compare>(first, last)`: 範囲 [first, last) を登録する。O(n log(n+1))。
`ordered_set<T, Compare>(first, last, comp)`: 範囲と比較関数を指定する。O(n log(n+1))。

関数:
`insert(x)`: x を挿入し、イテレータと挿入できたかの pair を返す。O(log(n+1))。
`erase(x)`: x と等価な要素を削除し、削除できたかを返す。O(log(n+1))。
`erase(it)`: it の要素を削除し、次のイテレータを返す。it は end() 以外。O(log(n+1))。
`find_by_order(k)`: 0始まりで k 番目のイテレータ。k >= size() なら end()。O(log(n+1))。
`order_of_key(x)`: Compare の順で x より前にある要素の個数。O(log(n+1))。
`find(x)`: x と等価な要素のイテレータ。なければ end()。O(log(n+1))。
`lower_bound(x)` / `upper_bound(x)`: std::set と同じ境界検索。O(log(n+1))。
`begin()` / `end()`: 双方向イテレータ。O(1)。
`rbegin()` / `rend()`: 逆順イテレータ。O(1)。
`size()` / `empty()`: 要素数・空判定。O(1)。`clear()`: 全削除。O(n)。

使用例:
```cpp
koma::ordered_set<> s;
s.insert(5);
s.insert(2);
s.insert(5);                // {2, 5}
auto x = *s.find_by_order(1); // 5
auto k = s.order_of_key(5);   // 1
s.erase(2);                  // {5}
```
*/
template <class T = long long, class Compare = std::less<T>>
using ordered_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, Compare, __gnu_pbds::rb_tree_tag,
									 __gnu_pbds::tree_order_statistics_node_update>;

} // namespace koma
