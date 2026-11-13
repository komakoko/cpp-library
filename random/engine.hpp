#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
ランダムテスト用の共通の64ビット疑似乱数生成器。
初期シードは0。`rand_engine.seed(seed)` で初期化する。
`rand_ll` と `rand_string` はこの生成器を共有する。
`std::shuffle(a.begin(), a.end(), koma::rand_engine)` にも使える。
*/
inline std::mt19937_64 rand_engine{0};

} // namespace koma
