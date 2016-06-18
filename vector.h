#include <vector>
#include <algorithm>
typedef unsigned long long ui64;

struct Vector {
    Vector(size_t n = 0, ui64 el = 0);
    Vector(Vector const &other);

    void resize(size_t n);
    void push_back(ui64 x);
    ui64 pop_back();
    ui64& back();
    void reverse();
    bool empty();
    size_t size() __attribute__((always_inline));

    Vector& operator=(Vector const &vec);
    ui64& operator[](const size_t k) __attribute__((always_inline));
    ui64 operator[](const size_t k) const __attribute__((always_inline));

private:
    std::vector<ui64> a;
    ui64 small_a;
    bool is_long;
    size_t length;
};

inline size_t Vector::size() {
    return length;
}

inline ui64& Vector::operator[](const size_t k) {
    return (is_long ? a[k] : small_a);
}

inline ui64 Vector::operator[](const size_t k) const {
    return (is_long ? a[k] : small_a);
}
