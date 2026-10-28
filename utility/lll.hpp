#pragma once
#include <bits/stdc++.h>

namespace koma {

using lll = __int128_t;
using ulll = __uint128_t;

inline std::ostream &operator<<(std::ostream &os, lll value) {
	bool negative = value < 0;
	__uint128_t magnitude = static_cast<__uint128_t>(value);
	if(negative) magnitude = -magnitude;
	std::string s;
	do {
		s.push_back('0' + magnitude % 10);
		magnitude /= 10;
	} while(magnitude);
	if(negative) s.push_back('-');
	std::reverse(s.begin(), s.end());
	return os << s;
}

inline std::istream &operator>>(std::istream &is, lll &value) {
	std::istream::sentry sentry(is);
	if(!sentry) return is;
	auto *buf = is.rdbuf();
	using traits = std::char_traits<char>;
	auto c = buf->sgetc();
	bool negative = c == '-';
	if(c == '-' || c == '+') c = buf->snextc();
	__uint128_t limit = (__uint128_t(1) << 127) - !negative;
	__uint128_t magnitude = 0;
	bool digits = false, overflow = false;
	while(c >= '0' && c <= '9') {
		digits = true;
		unsigned digit = c - '0';
		if(magnitude > (limit - digit) / 10) overflow = true;
		if(!overflow) magnitude = magnitude * 10 + digit;
		c = buf->snextc();
	}
	if(digits && !overflow) {
		value = negative ? -static_cast<lll>(magnitude - (magnitude != 0)) - (magnitude != 0) : static_cast<lll>(magnitude);
	}
	std::ios::iostate state = std::ios::goodbit;
	if(traits::eq_int_type(c, traits::eof())) state |= std::ios::eofbit;
	if(!digits || overflow) state |= std::ios::failbit;
	if(state != std::ios::goodbit) is.setstate(state);
	return is;
}

inline std::ostream &operator<<(std::ostream &os, ulll value) {
	std::string s;
	do {
		s.push_back('0' + value % 10);
		value /= 10;
	} while(value);
	std::reverse(s.begin(), s.end());
	return os << s;
}

inline std::istream &operator>>(std::istream &is, ulll &value) {
	std::istream::sentry sentry(is);
	if(!sentry) return is;
	auto *buf = is.rdbuf();
	using traits = std::char_traits<char>;
	auto c = buf->sgetc();
	bool negative = c == '-';
	if(c == '-' || c == '+') c = buf->snextc();
	ulll limit = ~ulll(0), magnitude = 0;
	bool digits = false, overflow = false;
	while(c >= '0' && c <= '9') {
		digits = true;
		unsigned digit = c - '0';
		if(magnitude > (limit - digit) / 10) overflow = true;
		if(!overflow) magnitude = magnitude * 10 + digit;
		c = buf->snextc();
	}
	if(digits && !overflow && !negative) value = magnitude;
	std::ios::iostate state = std::ios::goodbit;
	if(traits::eq_int_type(c, traits::eof())) state |= std::ios::eofbit;
	if(!digits || overflow || negative) state |= std::ios::failbit;
	if(state != std::ios::goodbit) is.setstate(state);
	return is;
}

}
