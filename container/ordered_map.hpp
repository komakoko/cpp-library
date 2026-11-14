#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace koma {

/**
キーに値を対応付け、キーの順位からの要素取得と、指定キーより前にある要素数の取得ができる。
GNU PBDS が必要。Key と Value の既定は long long、Compare の既定は std::less<Key>。
Compare は厳密弱順序を満たすこと。順位はキーの種類数で数える。
GNU PBDS の tree の操作をそのまま使える。

コンストラクタ:
`ordered_map<Key, Value, Compare>()`: 空のmapを作る。
`ordered_map<Key, Value, Compare>(comp)`: 比較関数 comp を指定して空のmapを作る。
`ordered_map<Key, Value, Compare>(first, last)`: {キー, 値} の範囲 [first, last) を登録する。O(n log(n+1))。
`ordered_map<Key, Value, Compare>(first, last, comp)`: 範囲と比較関数を指定する。O(n log(n+1))。

関数:
`operator[](key)`: 対応する値への参照。未登録なら値を値初期化して挿入する。O(log(n+1))。
`insert({key, value})`: 挿入位置と挿入できたかの pair を返す。既存キーの値は変更しない。O(log(n+1))。
`erase(key)`: キーの要素を削除し、削除できたかを返す。O(log(n+1))。
`erase(it)`: it の要素を削除し、次のイテレータを返す。it は end() 以外。O(log(n+1))。
`find_by_order(k)`: キー順で0始まりの k 番目のイテレータ。k >= size() なら end()。O(log(n+1))。
`order_of_key(key)`: Compare の順で key より前にあるキーの個数。O(log(n+1))。
`find(key)`: 要素のイテレータ。なければ end()。O(log(n+1))。
`lower_bound(key)` / `upper_bound(key)`: std::map と同じ境界検索。O(log(n+1))。
`begin()` / `end()` / `rbegin()` / `rend()`: キー順または逆順の双方向イテレータ。O(1)。
`size()` / `empty()`: キー数・空判定。O(1)。`clear()`: 全削除。O(n)。

使用例:
```cpp
koma::ordered_map<int, std::string> mp;
mp[5] = "five";
mp[2] = "two";
auto it = mp.find_by_order(1); // {5, "five"} を指す
auto k = mp.order_of_key(5);  // 1
it->second = "FIVE";          // 値は変更できる
```
*/
template <class Key = long long, class Value = long long, class Compare = std::less<Key>>
using ordered_map = __gnu_pbds::tree<Key, Value, Compare, __gnu_pbds::rb_tree_tag,
									 __gnu_pbds::tree_order_statistics_node_update>;

} // namespace koma
