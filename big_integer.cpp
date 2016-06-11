//
// Created by Appolinariya on 03.06.16.
//

#include "big_integer.h"

big_integer::big_integer() {
    size = 1;
	number = new bool[1];
    number[0] = 0;
    sign = false;
	bag();
}

big_integer::big_integer(const big_integer &right) {
    size = right.size;
    sign = right.sign;
	number = new bool[size];
    for (int i = 0; i < size; i++)
        number[i] = right.number[i];
    bag();
}

big_integer::big_integer(const int& num) {
    size = 0;
    sign = false;
    int mynum = num;
    if (num < 0) {
        sign = true;
        mynum = -mynum;
    }
	number = new bool[35];
    while(mynum) {
        number[size] = (bool(mynum % 2));
        size++;
        mynum /= 2;
    }
    number[size] = 0;
    size++;
    bag();
}

big_integer::big_integer(string s) {

    big_integer();
    if (!s.empty()) {
        size_t pos = 0;
        unsigned sign_rhs = 0;
        if (s[pos] == '-') {
            ++pos;
            sign_rhs = ~sign_rhs;
        }
        if (pos != s.length()) {
            for (; pos < s.length(); ++pos) {
                *this = (*this * 10) + (s[pos] - '0');
            }
            if ((sign_rhs) && (*this != 0)) {
                sign = sign_rhs;
                bag();
            }
        }
    }
}

big_integer& big_integer::operator=(const big_integer& right) {
    if(this == &right)
        return *this;
    size = right.size;
    sign = right.sign;
    for (int i = 0; i < size; i++) {
        number[i] = right.number[i];
    }
    bag();
    return *this;
}

bool operator==(const big_integer& left, const big_integer& right) {
    if (left.sign != right.sign || left.size != right.size)
        return false;
    for (int i = 0; i < left.size; i++) {
        if (left.number[i] != right.number[i])
            return false;
    }
    return true;
}

bool operator!=(const big_integer& left, const big_integer& right) {
    return !(left == right);
}

bool operator<(const big_integer& left, const big_integer& right) {
    if (left.sign > right.sign)
        return true;
    if (left.sign < right.sign)
        return false;
    if (left.sign) {
        if (left.size == right.size) {
            for (int i = left.size - 1; i >= 0; i--)
                if (left.number[i] > right.number[i])
                    return true;
                else if (left.number[i] < right.number[i])
                    return false;
            return false;
        } else
            return left.size > right.size;
    } else {
        if (left.size == right.size) {
            for (int i = left.size - 1; i >= 0; i--)
                if (left.number[i] < right.number[i])
                    return true;
                else if (left.number[i] > right.number[i])
                    return false;
            return false;
        } else
            return left.size < right.size;
    }
}

bool operator>(const big_integer& left, const big_integer& right) {
    if (left.sign < right.sign)
        return true;
    if (left.sign > right.sign)
        return false;
    if (left.sign) {
        if (left.size == right.size) {
            for (int i = left.size - 1; i >= 0; i--)
                if (left.number[i] < right.number[i])
                    return true;
                else if (left.number[i] > right.number[i])
                    return false;
            return false;
        } else
            return left.size < right.size;
    } else {
        if (left.size == right.size) {
            for (int i = left.size - 1; i >= 0; i--)
                if (left.number[i] > right.number[i])
                    return true;
                else if (left.number[i] < right.number[i])
                    return false;
            return false;
        } else
            return left.size > right.size;
    }
}

bool operator<=(const big_integer& left, const big_integer& right) {
    return !(left > right);
}

bool operator>=(const big_integer& left, const big_integer& right) {
    return !(left < right);
}

pair<bool, bool> sum(bool a, bool b, bool c) {
    return make_pair(a^b^c, (a & b) | (b & c) | (a & c));
};

const big_integer operator+(const big_integer& left, const big_integer& right) {
    big_integer ans;
    if (left.sign == right.sign) {
        ans.sign = left.sign;
        bool c = 0;
        ans.size = max(left.size, right.size);
        for (int i = 0; i < max(left.size, right.size); i++) {
            pair <bool, bool> res_sum = sum((bool) (i < left.size ? left.number[i] : 0), (bool) (i < right.size ? right.number[i] : 0), c);
            ans.number[i] = res_sum.first;
            c = res_sum.second;
        }
        if (c)
            ans.number[ans.size++] = c;
    } else {
        if (left.sign) {
            ans = left;
            ans.sign = false;
            ans = right - ans;
        } else {
            ans = right;
            ans.sign = false;
            ans = left - ans;
        }
    }
    ans.bag();
    return ans;
}

pair<bool, bool> sub(bool a, bool b, bool c) {
    return make_pair(a^b^c, ((b | c) & (!a)) |(b & a & c));
}

const big_integer operator-(const big_integer& left, const big_integer& right) {
    big_integer ans;
    big_integer left_ = left, right_ = right;
    left_.bag();
    right_.bag();
    if (left_.sign == right_.sign) {
        if (left_.sign) {
            left_.sign = false;
            right_.sign = false;
            ans = right_ - left_;
        } else {
            if (left_ < right_) {
                ans  = right_ - left_;
                ans.sign = true;
            } else {
                bool c = 0;
                pair<bool, bool> r;
                ans.size = left_.size;
                ans.change_capacity(ans.size);
                for (int i = 0; i < right_.size; i++) {
                    r = sub(left_.number[i], right_.number[i], c);
                    ans.number[i] = r.first;
                    c = r.second;
                }
                int i = right_.size;
                while (c || i != ans.size) {
                    r = sub(left_.number[i], 0, c);
                    ans.number[i] = r.first;
                    c = r.second;
                    i++;
                }
            }
        }
    } else {
        if (left_.sign) {
            big_integer left_ = left_;
            left_.sign = false;
            ans = left_ + right_;
            ans.sign = true;
        } else {
            ans = right_;
            ans.sign = false;
            ans = ans + left_;
        }
    }
    ans.bag();
    return ans;
}

const big_integer operator*(const big_integer& left, const big_integer& right) {
    big_integer ans;
    ans.size = right.size * left.size;
    pair<bool, bool> r;
    bool c;
    ans.sign = (left.sign != right.sign);
	
    ans.change_capacity(ans.size);
    for (int i = 0; i < right.size; i++) {
        c = false;
        for (int j = 0; j < left.size; j++) {
            r = sum(ans.number[i + j], left.number[j] * right.number[i], c);
            ans.number[i + j] = r.first;
            c = r.second;
        }
		unsigned k = i + left.size;
        while (c) {
            r = sum(ans.number[k], 0, c);
            ans.number[k] = r.first;
            c = r.second;
            k++;
        }
    }
    ans.bag();
    return ans;
}

const big_integer operator/(const big_integer& left, const big_integer& right) {
    big_integer ans;
    big_integer one(1);
    big_integer this_ = left;
    big_integer tmp2 = right;
    this_.sign = false;
    tmp2.sign = false;
    while (this_ >= tmp2) {
        ans = ans + one;
        this_ = this_ - tmp2;
    }
    ans.sign = (left.sign != right.sign);
    ans.bag();
    return ans;
}

const big_integer operator%(const big_integer& left, const big_integer& right) {
    big_integer ans;
    big_integer this_ = left;
    big_integer tmp2 = right;
    this_.sign = false;
    tmp2.sign = false;
    while (this_ >= tmp2) {
        this_ = this_ - tmp2;
    }
    ans = this_;
    ans.sign = left.sign;
    ans.bag();
    return ans;
}

big_integer& operator+=(big_integer& left, const big_integer& right) {
    left = left + right;
    return left;
}

big_integer& operator-=(big_integer& left, const big_integer& right) {
    left = left - right;
    return left;
}

big_integer& operator*=(big_integer& left, const big_integer& right) {
    left = left * right;
    return left;
}

big_integer& operator/=(big_integer& left, const big_integer& right) {
    left = left / right;
    return left;
}

big_integer& operator%=(big_integer& left, const big_integer& right) {
    left = left % right;
    return left;
}

const big_integer operator&(const big_integer& left, const big_integer& right) {
    big_integer ans;
    if (left.sign && right.sign) {
        big_integer left_ = left, right_ = right;
        left_.to_twos_complement(max(left.size, right.size) + 5);
        right_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
        ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            ans.number[i] = left_.number[i] & right_.number[i];
        ans.out_twos_complement(max(left.size, right.size) + 5);
    } else if (left.sign) {
        big_integer left_ = left;
        left_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
        ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            i < right.size ? ans.number[i] = left_.number[i] & right.number[i] : ans.number[i] = 0;
        ans.sign = false;
    } else if (right.sign) {
        big_integer right_ = right;
        right_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
        ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            i < left.size ? ans.number[i] = left.number[i] & right_.number[i] : ans.number[i] = 0;
        ans.sign = false;
    } else {
        ans.sign = false;
        ans.size = min(left.size, right.size);
        
		ans.change_capacity(ans.size);
        for (int i = 0; i < ans.size; i++)
            ans.number[i] = left.number[i] & right.number[i];
    }
    ans.bag();
    return ans;
}

const big_integer operator|(const big_integer& left, const big_integer& right) {
    big_integer ans;
    if (left.sign && right.sign) {
        big_integer left_ = left, right_ = right;
        left_.to_twos_complement(max(left.size, right.size) + 5);
        right_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
		
        ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            ans.number[i] = left_.number[i] | right_.number[i];
        ans.out_twos_complement(max(left.size, right.size) + 5);
        ans.sign = true;
    } else if (left.sign) {
        big_integer left_ = left;
        left_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
		
		ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            i < right.size ? ans.number[i] = left_.number[i] | right.number[i] : ans.number[i] = left_.number[i];
        ans.out_twos_complement(ans.size);
        ans.sign = true;
    } else if (right.sign) {
        big_integer right_ = right;
        right_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
		
		ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            i < left.size ? ans.number[i] = left.number[i] | right_.number[i] : ans.number[i] = right_.number[i];
        ans.out_twos_complement(ans.size);
        ans.sign = true;
    } else {
        ans.sign = false;
        ans.size = max(left.size, right.size);
		
		ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            if (i < left.size && i < right.size)
                ans.number[i] = left.number[i] | right.number[i];
            else if (i < left.size)
                ans.number[i] = left.number[i];
            else
                ans.number[i] = right.number[i];
    //    ans.number.push_back(0);
    }
    ans.bag();
    return ans;
}

const big_integer operator^(const big_integer& left, const big_integer& right) {
    big_integer ans;
    if (left.sign && right.sign) {
        big_integer left_ = left, right_ = right;
        left_.to_twos_complement(max(left.size, right.size) + 5);
        right_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
        ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            ans.number[i] = left_.number[i] ^ right_.number[i];
        ans.sign = false;
    } else if (left.sign) {
        big_integer left_ = left;
        left_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
        ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            i < right.size ? ans.number[i] = left_.number[i] ^ right.number[i] : ans.number[i] = left_.number[i];
        ans.out_twos_complement(ans.size);
        ans.sign = true;
    } else if (right.sign) {
        big_integer right_ = right;
        right_.to_twos_complement(max(left.size, right.size) + 5);
        ans.size = max(left.size, right.size) + 5;
		
		ans.change_capacity(ans.size);
        
        for (int i = 0; i < ans.size; i++)
            i < left.size ? ans.number[i] = left.number[i] ^ right_.number[i] : ans.number[i] = right_.number[i];
        ans.out_twos_complement(ans.size);
        ans.sign = true;
    } else {
        ans.sign = false;
        ans.size = max(left.size, right.size);
        
        for (int i = 0; i < ans.size; i++)
            if (i < left.size && i < right.size)
                ans.number[i] = left.number[i] ^ right.number[i];
            else if (i < left.size)
                ans.number[i] = left.number[i];
            else
                ans.number[i] = right.number[i];
        //ans.number.push_back(0);
    }
    ans.bag();
    return ans;
}

big_integer& operator&=(big_integer& left, const big_integer& right) {
    left = left & right;
    return left;
}

big_integer& operator|=(big_integer& left, const big_integer& right) {
    left = left | right;
    return left;
}

big_integer& operator^=(big_integer& left, const big_integer& right) {
     left = left ^ right;
    return left;
}

const big_integer operator~(const big_integer& tmp) {
    big_integer ans = -tmp - 1;
    ans.bag();
    return ans;
}

const big_integer operator-(const big_integer& tmp) {
    big_integer ans = tmp;
    ans.sign = !ans.sign;
    ans.bag();
    return ans;
}

const big_integer operator+(const big_integer& tmp) {
    return tmp;
}

const big_integer& operator++(big_integer& tmp) {
    big_integer one(1);
    tmp = tmp + one;
    return tmp;
}

const big_integer& operator--(big_integer& tmp) {
    big_integer one(1);
    tmp = tmp - one;
    return tmp;
}

const big_integer operator<<(big_integer const& tmp, int left) {
    big_integer ans = tmp, two(2);
    for (int i = 1; i <= left; i++)
        ans *= two;
    ans.bag();
    return ans;
}

const big_integer operator>>(big_integer const& tmp, int right) {
    big_integer ans = tmp, two(2);
    if (!tmp.sign) {
        for (int i = 1; i <= right; i++)
            ans /= two;
        ans.bag();
        return ans;
    } else {
        ans.to_twos_complement(ans.size + right);
        for (int i = 1; i <= right; i++)
            ans /= two;
        ans.out_twos_complement(ans.size + right);
        ans.sign = true;
        ans.bag();
        return ans;
    }
}

big_integer& operator<<=(big_integer& tmp, int left) {
    tmp = tmp << left;
    return tmp;
}

big_integer& operator>>=(big_integer& tmp, int right) {
    tmp = tmp >> right;
    return tmp;
}

int big_integer::toInt()const {
        int res = 0;
        for (int i = 0; i < size; i++) {
            res += number[i] * (1 << i);
        }
        if (sign)
            return -res;
        return res;
}

std::string to_string(big_integer const& tmp) {
    string ans;
    big_integer right = tmp;
    right.sign = false;
    big_integer ten(10);
    big_integer q(1);
    big_integer zero(0);
    while (!(tmp / q == zero)) {
        q = q * ten;
        big_integer tmp2 = ((right % q) / (q / ten));
        ans.push_back((char) ('0' + (tmp2.toInt())));
    }

    if (tmp.sign)
        ans.push_back('-');
    reverse(ans.begin(), ans.end());
    return ans;
}

void big_integer::bag() {
    while (size >= 2 && !number[size - 1]) {
        size--;
    }
	change_capacity(size);
    if (size == 1 && !number[0])
        sign = false;
}

void big_integer::to_twos_complement(int n) {
    sign = false;
	unsigned j = size;
	change_capacity(max(size, (unsigned)n));
    for (int i = 0; i < n; i++) {
        if (i >= j) {
            number[j] = 1;
            j++;
        } else
            number[i] = (bool) (1 - number[i]);
    }
    *this += 1;
    while (n > j) {
        number[j] = 1;
        j++;
    }
}

void big_integer::out_twos_complement(int n) {
    *this += -1;
    sign = true;

    for (int i = 0; i < n; i++)
        if (i < size)
            number[i] = !number[i];
    bag();
}

void big_integer::copy(bool *in, bool *out, int n) {
	for (int i = 0; i < n; i++)
		out[i] = in[i];
}

void big_integer::change_capacity(unsigned newCapacity) {
	bool *tmp = new bool[size];
	copy(number, tmp, size);
	number = new bool[newCapacity];
	for (int i = 0; i < newCapacity; i++)
		number[i] = 0;
	copy(tmp, number, size);
}
    