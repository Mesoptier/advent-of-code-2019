#pragma once

template<class int_t, int_t mod>
class mod_int
{
    using self_t = mod_int<int_t, mod>;

    int_t x;

public:
    mod_int() : x(0) {}
    mod_int(int_t x) : x(x) {}

    explicit operator int_t() const {
        return x;
    }

    self_t& operator=(int_t other) {
        x = other;
        return *this;
    }

    self_t& operator+=(const self_t& rhs) {
        x = (x + rhs.x) % mod;
        return *this;
    }
    friend self_t operator+(self_t lhs, const self_t& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend self_t operator-(self_t lhs, const self_t& rhs) {
        lhs += (mod - rhs.x);
        return lhs;
    }

    self_t& operator*=(const self_t& rhs) {
        int_t a = x;
        int_t b = rhs.x;
        x = 0;
        while (b > 0) {
            if (b & 1) {
                (*this) += a;
            }
            b = b >> 1;
            a = (2 * a) % mod;
        }
        return *this;
    }
    friend self_t operator*(self_t lhs, const self_t& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend self_t pow(self_t a, const self_t& b) {
        self_t res(1);
        int_t y = b.x;
        while (y > 0) {
            if (y & 1) {
                res *= a;
            }
            y = y >> 1;
            a *= a;
        }
        return res;
    }
};