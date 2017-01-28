#include "big_integer.h"
#include <sstream>
#include <stddef.h>
#include <iostream>

big_integer::big_integer(const big_integer &other) : box(other.box) {
}

big_integer::big_integer(ui64 a) : box(std::shared_ptr<Vector>(new Vector())) {
	    box->push_back(a);
        box->push_back(0);
}

big_integer::big_integer(int a) : box(std::shared_ptr<Vector>(new Vector())) {
	bool is_negative = (a < 0);
	ui64 aa;
    if (a < 0) {
        long long tmp = a;
	    aa = (ui64) (-tmp);
    } else
        aa = (ui64)a;
	    box->push_back(aa);
        box->push_back(0);
    if (is_negative) {
		*this = -(*this);
	}
}

big_integer::big_integer(const std::string &s) {
	const ui64 local_base(1000000000);
	const int base_digits = 9;
	Vector a;
	int pos = (s[0] == '-' || s[0] == '+') ? 1 : 0;
	for (int i = s.size() - 1; i >= pos; i -= 9) {
		ui64 x = 0;
		for (int j = std::max(pos, i - base_digits + 1); j <= i; j++)
			x = x * 10 + s[j] - '0';
		a.push_back(x);
	}

	while (a.size() > 0 && !a.back()) {
		a.pop_back();
	}
	if (a.empty()) {
		box = std::shared_ptr<Vector>(new Vector());
		box->push_back(0);
		box->push_back(0);
		return;
	}

	box = std::shared_ptr<Vector>(new Vector());
	while (!a.empty()) {
		ui64 rem = 0;
		for (size_t i = a.size(); i != 0; --i) {
			size_t j = i - 1;
			ui64 cur = a[j] + rem * local_base;
			a[j] = (ui64) (cur / base);
			rem = (ui64) (cur % base);
		}
		box->push_back(rem);
		while (!a.empty() && !a.back()) {
			a.pop_back();
		}
	}
	while (box->size() > 1 && !box->back()) {
		box->pop_back();
	}
	box->push_back(0);
	if (s[0] == '-') {
		*this = -(*this);
	}
}

big_integer& big_integer::operator=(big_integer const &rhs) {
	box = rhs.box;
	return *this;
}

big_integer& big_integer::operator+=(big_integer const &rhs) {
	return (*this = *this + rhs);
}

big_integer& big_integer::operator-=(big_integer const &rhs) {
	return (*this = *this - rhs);
}

big_integer& big_integer::operator*=(big_integer const &rhs) {
	return (*this = (*this) * rhs);
}

std::pair<big_integer, big_integer> divmod(big_integer a, big_integer b) {
	big_integer res_div, res_mod;
	Vector vec_div;

	ui64 res_sign = (a.box->back() ^ b.box->back());
	ui64 y = b.box->back();
	a = abs(a);
	b = abs(b);

	int col = a.box->size() - b.box->size();
	if (col > 0) {
		b <<= col * 32;
	}
	for (int i = 0; i <= col; ++i) {
		if (b <= a) {
			ui64 l = 0, r = big_integer::base;
			while (r - l > 1) {
				ui64 m = (l + r) >> 1;
				if (b * m <= a) {
					l = m;
				} else {
					r = m;
				}
			}
			    vec_div.push_back(l);
            a -= b * l;
		} else {
            vec_div.push_back(0);
		}
		b >>= 32;
	}
	vec_div.reverse();
	res_mod = a;

	while (vec_div.size() > 1 && !vec_div.back()) {
		vec_div.pop_back();
	}
	if (vec_div.empty()) {
		res_div = 0;
	} else  {
        vec_div.push_back(0);
		res_div.box = std::shared_ptr<Vector>(new Vector(vec_div));
		if (res_sign) {
			res_div = -res_div;
			if (!y) res_mod = -res_mod;
		}
	}

	trim(res_div);
	trim(res_mod);
	return std::make_pair(res_div, res_mod);
}

big_integer& big_integer::operator/=(int rhs) {
	bool res_sign = (rhs < 0) ^ bool(box->back() & 1);
    ui64 urhs;
    if (rhs < 0) {
        long long tmp = rhs;
        urhs = (ui64) (-tmp);
    } else
        urhs = (ui64)rhs;
    ui64 rem = 0;
	*this = abs(*this);

	for (size_t i = this->box->size() - 1; i != 0; --i) {
		size_t j = i - 1;
		ui64 cur = this->box->operator[](j) + rem * base;
        this->box->operator[](j) = cur / urhs;
        rem = cur % urhs;
	}

	trim(*this);
	if (res_sign) *this = -(*this);
	return *this;
}

big_integer& big_integer::operator/=(big_integer const &rhs) {
	*this = divmod(*this, rhs).first;
	return *this;
}

big_integer& big_integer::operator%=(big_integer const &rhs) {
	*this = divmod(*this, rhs).second;
	return *this;
}

big_integer& big_integer::operator&=(big_integer const &rhs) {
	return (*this = *this & rhs);
}

big_integer& big_integer::operator|=(big_integer const &rhs) {
	return (*this = *this | rhs);
}

big_integer& big_integer::operator^=(big_integer const &rhs) {
	return (*this = *this ^ rhs);
}

big_integer& big_integer::operator<<=(int rhs) {
	return (*this = *this << rhs);
}

big_integer& big_integer::operator>>=(int rhs) {
	return (*this = *this >> rhs);
}

big_integer big_integer::operator++() {
	return (*this += 1);
}

big_integer big_integer::operator++(int i) {
	big_integer old_value = *this;
	*this += 1;
	return old_value;
}

big_integer big_integer::operator--() {
	return (*this -= 1);
}

big_integer big_integer::operator--(int i) {
	big_integer old_value = *this;
	*this -= 1;
	return old_value;
}

big_integer big_integer::operator+() const {
	return *this;
}

big_integer big_integer::operator-() const {
	big_integer b = *this;
	if (b == 0)
		return b;
	b = ~(b) + 1;
	return b;
}

void trim(big_integer &a) {
	while (a.box->size() > 2 && (a.box->operator[](a.box->size() - 1) == a.box->operator[](a.box->size() - 2))) {
            a.box->pop_back();
	}
}

big_integer abs(big_integer const &a) {
	big_integer b = a;
	if (b.box->back()) b = -b;
	return b;
}

big_integer big_integer::operator~() const {
	big_integer b;
	b.box->resize(this->box->size());

	for (size_t i = 0; i < b.box->size(); ++i) {
		b.box->operator[](i) = (~this->box->operator[](i)) & neg_sign;
	}

	trim(b);
	return b;
}

big_integer operator+(big_integer const &a, big_integer const &b) {
	big_integer aa = a, bb = b, res;

	ui64 t = 0;
	while (aa.box->size() < bb.box->size()) aa.box->push_back(aa.box->back());
	while (bb.box->size() < aa.box->size()) bb.box->push_back(bb.box->back());
	res.box->resize(aa.box->size());

	for (size_t i = 0; i < aa.box->size(); ++i) {
		t += aa.box->operator[](i) + bb.box->operator[](i);
		res.box->operator[](i) = t & big_integer::neg_sign;
		t >>= 32;
	}
	
	bool ts = t & 1, as = aa.box->back() & 1, bs = bb.box->back() & 1;
	res.box->push_back(ts ^ as ^ bs ? big_integer::neg_sign : 0);

	trim(aa);
	trim(bb);
	trim(res);

	return res;
}

big_integer operator-(big_integer const &a, big_integer const &b) {
	big_integer aa = a, bb = -b, res;
	res = aa + bb;
	return res;
}

big_integer operator*(big_integer const &a, big_integer const &b) {
	big_integer aa = a, bb = b, res;
	ui64 res_sign = (aa.box->back() ^ bb.box->back());
	aa = abs(aa);
	bb = abs(bb);

	res.box->resize(aa.box->size() + bb.box->size());
	for (size_t i = 0; i < aa.box->size(); ++i) {
		ui64 carry = 0;
		for (size_t j = 0; j < bb.box->size() || carry; ++j) {
			ui64 cur = res.box->operator[](i + j) + aa.box->operator[](i) * (j < bb.box->size() ? bb.box->operator[](j) : 0) + carry;
			res.box->operator[](i + j) = cur & big_integer::neg_sign;
			carry = cur >> 32;
		}
	}

	while (res.box->size() > 1 && !res.box->back()) {
		res.box->pop_back();
	}
	res.box->push_back(0);
	if (res_sign) res = -res;

	return res;
}

big_integer operator/(big_integer const &a, big_integer const &b) {
	big_integer res = divmod(a, b).first;
	return res;
}

big_integer operator%(big_integer const &a, big_integer const &b) {
	big_integer res = divmod(a, b).second;
	return res;
}

big_integer operator&(big_integer const &a, big_integer const &b) {
	big_integer aa = a, bb = b, res;

	while (aa.box->size() < bb.box->size()) aa.box->push_back(aa.box->back());
	while (bb.box->size() < aa.box->size()) bb.box->push_back(bb.box->back());
	res.box->resize(aa.box->size());

	for (size_t i = 0; i < aa.box->size(); ++i) {
		res.box->operator[](i) = aa.box->operator[](i) & bb.box->operator[](i);
	}

	trim(aa);
	trim(bb);
	trim(res);

	return res;
}

big_integer operator|(big_integer const &a, big_integer const &b) {
	big_integer aa = a, bb = b, res;

	while (aa.box->size() < bb.box->size()) aa.box->push_back(aa.box->back());
	while (bb.box->size() < aa.box->size()) bb.box->push_back(bb.box->back());
	res.box->resize(aa.box->size());

	for (size_t i = 0; i < aa.box->size(); ++i) {
		res.box->operator[](i) = aa.box->operator[](i) | bb.box->operator[](i);
	}

	trim(aa);
	trim(bb);
	trim(res);

	return res;
}

big_integer operator^(big_integer const &a, big_integer const &b) {
	big_integer aa = a, bb = b, res;

	while (aa.box->size() < bb.box->size()) aa.box->push_back(aa.box->back());
	while (bb.box->size() < aa.box->size()) bb.box->push_back(bb.box->back());
	res.box->resize(aa.box->size());

	for (size_t i = 0; i < aa.box->size(); ++i) {
		res.box->operator[](i) = aa.box->operator[](i) ^ bb.box->operator[](i);
	}

	trim(aa);
	trim(bb);
	trim(res);

	return res;
}

big_integer operator<<(big_integer const &a, int b) {
	big_integer aa = a;
	if (aa == 0 || b == 0) {
		return aa;
	}

	ui64 quick_shift = ui64((long long)b >> 5), slow_shift = 1U << ui64(((long long)b) & 31);
	aa.box->reverse();
	for (ui64 i = 0; i < quick_shift; ++i) {
		aa.box->push_back(0);
	}
	aa.box->reverse();

	aa *= slow_shift;

	return aa;
}

big_integer operator>>(big_integer const &a, int b) {
    big_integer aa = a;
	if (aa == 0 || b == 0) {
		return aa;
	}
	if ((b / 32) >= (int) (aa.box->size() - 1)) {
		aa = big_integer(0);
		return aa;
	}

	ui64 quick_shift = ui64(b >> 5), slow_shift = 1UL << (b & 31);
    aa.box->reverse();
	for (ui64 i = 0; i < quick_shift; ++i) {
		aa.box->pop_back();
	}
    aa.box->reverse();

	aa /= slow_shift;
    if (a.box->back())
        aa -= 1;
    else if (b == 31)
        aa = -aa;
    return aa;
}

bool operator==(big_integer const &a, big_integer const &b) {
	ui64 as = a.box->back(), bs = b.box->back();
	if ((a.box->size() != b.box->size()) || (as != bs)) {
		return 0;
	}

	bool f = true;
	for (size_t i = 0; i < a.box->size() && f; ++i) {
		f = (a.box->operator[](i) == b.box->operator[](i));
	}

	return f;
}

bool operator!=(big_integer const &a, big_integer const &b) {
	return (!(a == b));
}

bool operator<(big_integer const &a, big_integer const &b) {
	big_integer aa = a, bb = b;
	bool as = aa.box->back() & 1, bs = bb.box->back() & 1;

	if (as ^ bs) {
		return !bs;
	}
	
	bool cs = as;
	aa = abs(aa);
	bb = abs(bb);

	if (aa.box->size() != bb.box->size()) {
		return ((aa.box->size() < bb.box->size()) ^ cs);
	}

	ptrdiff_t i;
	for (i = (ptrdiff_t)(aa.box->size() - 1); (i >= 0) && (aa.box->operator[](i) == bb.box->operator[](i)); --i) {}

	if (i < 0) {
		return 0;
	}

	return ((aa.box->operator[](i) < bb.box->operator[](i)) ^ cs);
}

bool operator>(big_integer const &a, big_integer const &b) {
	return (!(a < b) && (a != b));
}

bool operator<=(big_integer const &a, big_integer const &b) {
	return ((a < b) || (a == b));
}

bool operator>=(big_integer const &a, big_integer const &b) {
	return ((a > b) || (a == b));
}

Vector trim_to_base(Vector const &vc, const ui64 to_base) {
	Vector vec = vc;
	size_t k = 0;
	while (k < vec.size()) {
		ui64 p = vec[k] / to_base, r = vec[k] % to_base;
		if (k == vec.size() - 1) {
			if (p) vec.push_back(p);
		} else {
			vec[k + 1] += p;
		}
		vec[k++] = r;
	}
	
	while (!vec.empty() && !vec.back()) {
		vec.pop_back();
	}
	return vec;
}

std::string to_string(big_integer const &a) {
	std::string s = "";
	big_integer b(a);
	if (b.box->back()) {
		s = "-";
		b = -b;
	}

	const ui64 local_base = 10000;
	const int base_digits = 4;
	Vector sum, pow;

	sum.push_back(b.box->operator[](0));
	pow.push_back(1);
	for (size_t j = 1; j < b.box->size() - 1; ++j) {
		for (size_t i = 0; i < pow.size(); ++i) {
			pow[i] *= big_integer::base;
		}	
		pow = trim_to_base(pow, local_base);
		
		Vector mul(pow.size());
		for (size_t i = 0; i < mul.size(); ++i) {
			mul[i] = pow[i] * b.box->operator[](j);
		}
		mul = trim_to_base(mul, local_base);

		for (size_t i = 0; i < mul.size(); ++i) {
			if (sum.size() == i) {
				sum.push_back(mul[i]);
			} else {
				sum[i] += mul[i];
			}
		}
		sum = trim_to_base(sum, local_base);
	}
	
	std::stringstream ss;
	std::string t;
	ss.clear();
	ss << sum.back();
	ss >> t;
	s += t;
	if (sum.size() > 1) {
		for (size_t i = sum.size() - 1; i != 0; --i) {
			size_t j = i - 1;
			ss.clear();
			ss << sum[j];
			ss >> t;
			while (t.length() < base_digits) {
				t = "0" + t;
			}
			s += t;
		}
	}
	return s;
}
