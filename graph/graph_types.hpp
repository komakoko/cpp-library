#pragma once
#include <bits/stdc++.h>

namespace koma {

using Graph = std::vector<std::vector<long long>>;

template <class W = long long, class I = long long>
using WeightedGraph = std::vector<std::vector<std::pair<I, W>>>;

} // namespace koma