#include <bits/stdint-uintn.h>
#include <iostream>
#include <sys/types.h>
#include <type_traits>
#include <array>
#include <cassert>
 
using std::size_t;

template <size_t SIZE>
using d = std::array<uint8_t, SIZE>;
static const uint8_t BASE = 2;
static const int NUM_BITS_IN_BYTE = 8;

template <size_t SIZE, bool SIGNED>
struct Number {

    Number(int n) {
        fill_data(n);
    }

    Number(uint n) {
        fill_data(n);
    }

    void fill_data(uint n) {
        for (size_t i = 0; i < SIZE; i++) {
            data_[i] =((1 << i) & n) > 0 ? 1 : 0;
        }
    }

    Number(const d<SIZE>& init_data):
        data_(init_data) {};

    template <size_t S2, bool B2>
    Number(Number<S2, B2> b) {
        static_assert(SIZE >= S2, "narrowing isn't supported!");
        fill_data(static_cast<int32_t>(b));
    }

    template<size_t _SIZE = SIZE, bool _SIGNED = SIGNED,
    std::enable_if_t<_SIGNED, bool> = true>
    auto negate() {
        d<SIZE> res_data;
        for (int i = SIZE - 1; i >= 0; i--) {
            res_data[i] = data_[i] == 1 ? 0 : 1;
        }

        Number <SIZE, SIGNED> one(1);
        return Number<SIZE, SIGNED>(res_data).add(one);
    }

    template<size_t _SIZE = SIZE, bool _SIGNED = SIGNED,
    std::enable_if_t<_SIGNED, bool> = true>
    auto subtract(Number<SIZE, SIGNED> n2) {
        auto neg_n2 = n2.negate();
        return add(neg_n2);
    }

    template <size_t S2, bool B2>
    Number<SIZE, SIGNED> add(Number<S2, B2> n2) {
        d<SIZE> res_data;
        uint8_t carry = 0;
        for (size_t i = 0; i < SIZE; i++) {
            uint8_t sum = data_[i] + n2.data_[i] + carry;
            res_data[i] = sum % BASE;
            carry = sum / BASE;
        }
        return Number<SIZE, SIGNED>(res_data);
    }

    template<size_t _SIZE = SIZE, bool _SIGNED = SIGNED,
    std::enable_if_t<_SIGNED, bool> = true>
    Number<SIZE, SIGNED> multiply(Number<SIZE, SIGNED> n2) {
        int32_t times = n2;
        Number<SIZE, SIGNED> res(0);
        auto adder = *this;
        if (times < 0) {
            adder = adder.negate();
            times = std::abs(times);
        }

        for (size_t i = 0; i < times; i++) {
            res = res.add(adder);
        }
        return res;
    }

    template<size_t _SIZE = SIZE, bool _SIGNED = SIGNED,
    std::enable_if_t<!_SIGNED, bool> = true>
    Number<SIZE, SIGNED> multiply(Number<SIZE, SIGNED> n2) {
        int32_t times = n2;
        Number<SIZE, SIGNED> res(0);
        for (size_t i = 0; i < times; i++) {
            res = res.add(*this);
        }
        return res;
    }

    operator int32_t() const {
        size_t NB = sizeof(int32_t) * NUM_BITS_IN_BYTE;
        int res = 0;
        for (size_t i = 0; i < NB; i++) {
            if (i < SIZE) {
                res |= (1 << i) * data_[i]; // copy bits into `res`
            } else if (SIGNED) {
                res |= (1 << i) * data_[SIZE - 1]; // sign-extension
            } else {
                break; // unsigned numbers, no need to sign extend
            }
        }
        return res;
    }

    Number<SIZE, SIGNED> operator<<(size_t n) {
        auto norm = n % SIZE;
        Number<SIZE, SIGNED> res(*this);
        for (size_t i = 0; i < norm; i++) {
            res = res.add(*this);
        }

        return res;
    }


    d<SIZE> data_;
};

template <size_t S, bool B>
std::ostream& operator<<(std::ostream& os, const Number<S, B>& num) {
    int32_t r = static_cast<int32_t>(num);
    if (B) {
        os << static_cast<int32_t>(r);
    }
    else {
        os << static_cast<uint>(r);
    }
    os << " (";
    for (int i = S - 1; i >=0; i--) {
        os << static_cast<int>(num.data_[i]);
    }
    os << ")";
    return os;
}

using int4  =  Number<4, true>;
using uint4 = Number<4, false>;
using int8  =  Number<8, true>;
using uint8 = Number<8, false>;
using int16  =  Number<16, true>;
using uint16 = Number<16, false>;

// 1. If the types are the same, that type is the common type.
// Else, the types are different:
// 2. If the types have the same signedness (both signed or both unsigned), the operand whose type has the lesser conversion rank1 is implicitly converted2 to the other type.
// Else, the operands have different signedness:
// 3. If the unsigned type has conversion rank greater than or equal to the rank of the signed type, then the operand with the signed type is implicitly converted to the unsigned type.
// 4. Else, the unsigned type has conversion rank less than the signed type:
// If the signed type can represent all values of the unsigned type, then the operand with the unsigned type is implicitly converted to the signed type.
// Else, both operands undergo implicit conversion to the unsigned type counterpart of the signed operand's type.
template <size_t S1, size_t S2, bool B1, bool B2,  std::enable_if_t<S1 == S2 && B1 == B2, bool> = true>
auto operator+(Number<S1, B1> n1, Number<S2, B2> n2) {
    //std::cout << "running case 1) S1 == S2 && B1 == B2\n";
    return Number<S1, B1>(n1.data_).add(n2);
}

template <size_t S1, size_t S2, bool B1, bool B2,  std::enable_if_t<B1 == B2 && S1 != S2, bool> = true>
auto operator+(Number<S1, B1> n1, Number<S2, B2> n2) {
    //std::cout << "running case 2) B1 == B2 && S1 != S2\n";
    Number<std::max(S1, S2), B1> n1_ext(static_cast<int32_t>(n1));
    Number<std::max(S1, S2), B1> n2_ext(static_cast<int32_t>(n2));
    return n1_ext.add(n2_ext);
}

template <size_t S1, size_t S2, bool B1, bool B2,  std::enable_if_t<B1 != B2 && ((!B1 && S1 >= S2) || (!B2 && S2 >= S1)), bool> = true>
auto operator+(Number<S1, B1> n1, Number<S2, B2> n2) {
    //std::cout << "running case 3) (!B1 && S1 >= S2) || (!B2 && S2 >= S1)\n";
    Number<S1, false> n2_ext(static_cast<int32_t>(n2));
    return n1.add(n2_ext);
}

template <size_t S1, size_t S2, bool B1, bool B2,  std::enable_if_t<B1 != B2 && ((B1 && S1 > S2) || (B2 && S2 > S1)), bool> = true>
auto operator+(Number<S1, B1> n1, Number<S2, B2> n2) {
    //std::cout << "running case 4) (!B1 && S1 >= S2) || (!B2 && S2 >= S1)\n";
    Number<S1, true> n2_ext(static_cast<int32_t>(n2));
    return n1.add(n2_ext);
}