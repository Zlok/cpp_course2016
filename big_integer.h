//
// Created by Appolinariya on 01.06.16.
//

#ifndef BIGINTEGER_BIG_INTEGER_H
#define BIGINTEGER_BIG_INTEGER_H

#include <string>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

class big_integer;

struct big_integer {
    bool sign;
private:
    bool *number;
	unsigned size;
private:
	void copy(bool *in, bool *out, int n);
	void change_capacity(unsigned newCapacity);
    void to_twos_complement(int n);
    void out_twos_complement(int n);
    void bag();
public:
    big_integer();
    big_integer(const big_integer& tmp);
    big_integer(const int& num);
    big_integer(string s);
	~big_integer() {
		for (int i = size - 1; i >= 0; i--)
			number[i] = NULL;
        delete(number);
		cout << size << " !" << endl;
	}
	int toInt()const;

    big_integer& operator=(const big_integer& right);

    friend bool operator==(const big_integer& left, const big_integer& right);
    friend bool operator!=(const big_integer& left, const big_integer& right);
    friend bool operator<(const big_integer& left, const big_integer& right);
    friend bool operator>(const big_integer& left, const big_integer& right);
    friend bool operator<=(const big_integer& left, const big_integer& right);
    friend bool operator>=(const big_integer& left, const big_integer& right);

private:
    pair<bool, bool> sum(bool a, bool b, bool c);
    pair<bool, bool> sub(bool a, bool b, bool c);

public:
    friend const big_integer operator+(const big_integer& left, const big_integer& right);
    friend const big_integer operator-(const big_integer& left, const big_integer& right);
    friend const big_integer operator*(const big_integer& left, const big_integer& right);
    friend const big_integer operator/(const big_integer& left, const big_integer& right);
    friend const big_integer operator%(const big_integer& left, const big_integer& right);

    friend big_integer& operator+=(big_integer& left, const big_integer& right);
    friend big_integer& operator-=(big_integer& left, const big_integer& right);
    friend big_integer& operator*=(big_integer& left, const big_integer& right);
    friend big_integer& operator/=(big_integer& left, const big_integer& right);
    friend big_integer& operator%=(big_integer& left, const big_integer& right);

    friend const big_integer operator&(const big_integer& left, const big_integer& right);
    friend const big_integer operator|(const big_integer& left, const big_integer& right);
    friend const big_integer operator^(const big_integer& left, const big_integer& right);

    friend big_integer& operator&=(big_integer& left, const big_integer& right);
    friend big_integer& operator|=(big_integer& left, const big_integer& right);
    friend big_integer& operator^=(big_integer& left, const big_integer& right);

    friend const big_integer operator~(const big_integer& tmp);

    friend const big_integer operator-(const big_integer& tmp);
    friend const big_integer operator+(const big_integer& tmp);
    friend const big_integer& operator--(big_integer& tmp);
    friend const big_integer& operator++(big_integer& tmp);

    friend const big_integer operator<<(big_integer const& tmp, int left);
    friend const big_integer operator>>(big_integer const& tmp, int right);

    friend big_integer& operator<<=(big_integer& tmp, int left);
    friend big_integer& operator>>=(big_integer& tmp, int right);
};

string to_string(big_integer const& tmp);

#endif //BIGINTEGER_BIG_INTEGER_H
