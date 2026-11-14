#pragma once
#include <bits/stdc++.h>

namespace koma {
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pll = std::pair<ll, ll>;
}

#define sz(a) static_cast<::koma::ll>((a).size())
#define KOMA_DETAIL_SELECT_LOOP(_1, _2, _3, _4, NAME, ...) NAME
#define KOMA_DETAIL_LOOP(i, l, r, step, cmp, advance) KOMA_DETAIL_LOOP_EXPAND(i, l, r, step, cmp, advance, __COUNTER__)
#define KOMA_DETAIL_LOOP_EXPAND(i, l, r, step, cmp, advance, id) KOMA_DETAIL_LOOP_IMPL(i, l, r, step, cmp, advance, id)
#define KOMA_DETAIL_LOOP_IMPL(i, l, r, step, cmp, advance, id) \
	for(::koma::ll i = (::koma::ll)(l), koma_loop_end_##id = (::koma::ll)(r), koma_loop_step_##id = (::koma::ll)(step); \
		(assert(koma_loop_step_##id > 0), i cmp koma_loop_end_##id); i advance koma_loop_step_##id)
#define KOMA_DETAIL_REPEAT(loop, n) KOMA_DETAIL_REPEAT_EXPAND(loop, n, __COUNTER__)
#define KOMA_DETAIL_REPEAT_EXPAND(loop, n, id) KOMA_DETAIL_REPEAT_IMPL(loop, n, id)
#define KOMA_DETAIL_REPEAT_IMPL(loop, n, id) loop(koma_repeat_##id, n)
#define KOMA_DETAIL_REP_1(n) KOMA_DETAIL_REPEAT(KOMA_DETAIL_REP_2, n)
#define KOMA_DETAIL_REP_2(i, n) KOMA_DETAIL_REP_3(i, 0, n)
#define KOMA_DETAIL_REP_3(i, l, r) KOMA_DETAIL_REP_4(i, l, r, 1)
#define KOMA_DETAIL_REP_4(i, l, r, step) KOMA_DETAIL_LOOP(i, l, r, step, <, +=)
#define KOMA_DETAIL_REP1_1(n) KOMA_DETAIL_REPEAT(KOMA_DETAIL_REP1_2, n)
#define KOMA_DETAIL_REP1_2(i, n) KOMA_DETAIL_REP1_3(i, 1, n)
#define KOMA_DETAIL_REP1_3(i, l, r) KOMA_DETAIL_REP1_4(i, l, r, 1)
#define KOMA_DETAIL_REP1_4(i, l, r, step) KOMA_DETAIL_LOOP(i, l, r, step, <=, +=)
#define KOMA_DETAIL_RREP_1(n) KOMA_DETAIL_REPEAT(KOMA_DETAIL_RREP_2, n)
#define KOMA_DETAIL_RREP_2(i, n) KOMA_DETAIL_RREP_3(i, 0, n)
#define KOMA_DETAIL_RREP_3(i, l, r) KOMA_DETAIL_RREP_4(i, l, r, 1)
#define KOMA_DETAIL_RREP_4(i, l, r, step) KOMA_DETAIL_LOOP(i, (::koma::ll)(r) - 1, l, step, >=, -=)
#define KOMA_DETAIL_RREP1_1(n) KOMA_DETAIL_REPEAT(KOMA_DETAIL_RREP1_2, n)
#define KOMA_DETAIL_RREP1_2(i, n) KOMA_DETAIL_RREP1_3(i, 1, n)
#define KOMA_DETAIL_RREP1_3(i, l, r) KOMA_DETAIL_RREP1_4(i, l, r, 1)
#define KOMA_DETAIL_RREP1_4(i, l, r, step) KOMA_DETAIL_LOOP(i, r, l, step, >=, -=)
#define rep(...) KOMA_DETAIL_SELECT_LOOP(__VA_ARGS__, KOMA_DETAIL_REP_4, KOMA_DETAIL_REP_3, KOMA_DETAIL_REP_2, KOMA_DETAIL_REP_1)(__VA_ARGS__)
#define rep1(...) KOMA_DETAIL_SELECT_LOOP(__VA_ARGS__, KOMA_DETAIL_REP1_4, KOMA_DETAIL_REP1_3, KOMA_DETAIL_REP1_2, KOMA_DETAIL_REP1_1)(__VA_ARGS__)
#define rrep(...) KOMA_DETAIL_SELECT_LOOP(__VA_ARGS__, KOMA_DETAIL_RREP_4, KOMA_DETAIL_RREP_3, KOMA_DETAIL_RREP_2, KOMA_DETAIL_RREP_1)(__VA_ARGS__)
#define rrep1(...) KOMA_DETAIL_SELECT_LOOP(__VA_ARGS__, KOMA_DETAIL_RREP1_4, KOMA_DETAIL_RREP1_3, KOMA_DETAIL_RREP1_2, KOMA_DETAIL_RREP1_1)(__VA_ARGS__)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
