#include "vector.h"

Vector::Vector(size_t n, ui64 el) : length(n) {
    if (n <= 1) {
        is_long = 0;
        num->small_a = el;
    } else {
        num->a.resize(n);
        is_long = true;
        for (size_t i = 0; i < n; ++i) {
            num->a[i] = el;
        }
    }
}

Vector::Vector(Vector const &other) : num(other.num), is_long(other.is_long), length(other.length) {
}

Vector::~Vector() {
    if (num != NULL)
        num = NULL;
}

void Vector::resize(size_t n) {
    if (is_long || (n > 1)) {
        num->a.resize(n);
        is_long = 1;
    } else {
        num->a.clear();
        is_long = 0;
        num->small_a = 0;
    }
    length = n;
}

void Vector::push_back(ui64 x) {
    if (!is_long) {
        if (length) {
            num->a.clear();
            num->a.push_back(num->small_a);
            num->a.push_back(x);
            is_long = 1;
            num->small_a = 0;
        } else {
            num->small_a = x;
        }
    } else {
        num->a.push_back(x);
    }
    length++;
}

ui64 Vector::pop_back() {
    ui64 t = this->back();
    if (is_long) {
        num->a.pop_back();
        if (num->a.size() == 1) {
            num->small_a = num->a[0];
            is_long = 0;
            num->a.clear();
        }
    }
    length--;
    return t;
}

ui64& Vector::back() {
    return (is_long ? num->a.back() : num->small_a);
}

void Vector::reverse() {
    if (is_long) std::reverse(num->a.begin(), num->a.end());
}

bool Vector::empty() {
    return (length == 0);
}

Vector& Vector::operator=(Vector const &other) {
    num = other.num;
    is_long = other.is_long;
    length = other.length;
    return *this;
}
