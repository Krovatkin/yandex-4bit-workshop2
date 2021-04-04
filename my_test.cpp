#include "gtest/gtest.h"
#include "number.hpp"

TEST(IntegerTests, Addition) { ASSERT_EQ(2 + 2, 4); }

TEST(Number, SimpeAdd) {

    int4 m{-3};
    int4 m2{2};
    std::cout << "m = " << m << std::endl;
    std::cout << "m2 = " << m.multiply(m2) << std::endl;
}

TEST (Number, UnsignedAdd) {
    for (uint32_t ua = 0; ua < 256; ua++) {
        for (uint32_t ub = 0; ub < 256; ub++) {
            int8_t a = static_cast<uint8_t>(ua), b = static_cast<uint8_t>(ub);
            //std::cout << "running a = " << static_cast<uint32_t>(a) << ", b = " << static_cast<uint32_t>(b) << std::endl;
            ASSERT_EQ(static_cast<uint32_t>(static_cast<uint8_t>(a + b)), static_cast<int32_t>(uint8{a} + uint8{b}));
        }
    }
}

TEST (Number, UnsignedMultiply) {
    for (uint32_t ua = 0; ua < 256; ua++) {
        for (uint32_t ub = 0; ub < 256; ub++) {
            int8_t a = static_cast<uint8_t>(ua), b = static_cast<uint8_t>(ub);
            ASSERT_EQ(static_cast<uint32_t>(static_cast<uint8_t>(a * b)), static_cast<int32_t>(uint8{a}.multiply(uint8{b})));
        }
    }
}

TEST (Number, SignedAdd) {
    for (uint32_t ua = 0; ua < 256; ua++) {
        for (uint32_t ub = 0; ub < 256; ub++) {
            int8_t a = static_cast<int8_t>(ua), b = static_cast<int8_t>(ub);  
            ASSERT_EQ(static_cast<int32_t>(static_cast<int8_t>(a + b)), static_cast<int32_t>(int8{a} + int8{b}));
        }
    }
}

TEST (Number, SignedMultiply) {
    for (uint32_t ua = 0; ua < 256; ua++) {
        for (uint32_t ub = 0; ub < 256; ub++) {
            int8_t a = static_cast<int8_t>(ua), b = static_cast<int8_t>(ub);
            ASSERT_EQ(static_cast<int32_t>(static_cast<int8_t>(a * b)), static_cast<int32_t>(int8{a}.multiply(int8{b})));
        }
    }
}

TEST (Number, UnsignedExtend) {
    for (uint32_t ua = 0; ua < 256; ua++) {
            uint8 a(ua);
            ASSERT_EQ(static_cast<int32_t>(uint16(uint8(ua))), static_cast<uint16_t>(static_cast<uint8_t>(ua)));
    }
}

TEST (Number, SignedExtend) {
    for (uint32_t ua = 0; ua < 256; ua++) {
            ASSERT_EQ(static_cast<int32_t>(int16(int8(ua))), static_cast<int16_t>(static_cast<int8_t>(ua)));
    }
}


// TEST (Number, MismatchMultiply2) {
//     for (uint32_t ua = 0; ua < 256; ua++) {
//         for (uint32_t ub = 0; ub < 256; ub++) {
//             auto ia = static_cast<int8_t>(ua);
//             auto a = static_cast<uint8_t>(ua);
//             auto b = static_cast<uint8_t>(ub);
//             //std::cout << " a = " << (uint32_t)a << " b = " << (uint32_t)b << std::endl;
//             ASSERT_EQ (static_cast<int8_t>(a * b) , static_cast<int8_t>(ia * b)) ;
//             //ASSERT_EQ(static_cast<int32_t>(int16(int8(ua))), static_cast<int16_t>(static_cast<int8_t>(ua)));
//         }
//     }
// }


// std::cout << "a " << (int32_t)a << " b = " << (int32_t) b << std::endl;
/*


    // int4 i1{-2};
    // uint ui2{1};

    // int4 m{2};
    // auto m_neg = m.negate();
    // std::cout << "m = " << m_neg << std::endl;
    // int4 m2{3};
    // std::cout << "m2 = " << m2 << std::endl;
    // auto m3 = m2 + m_neg;
    // std::cout << m3 << std::endl;

    // uint8_t a = 254, b = 254;
    // std::cout << static_cast<int32_t>(uint8{a}.multiply(uint8{b})) << std::endl;
    // std::cout << static_cast<uint32_t>(static_cast<uint8_t>(a * b)) << std::endl;
    //assert(static_cast<uint32_t>(a + b) == static_cast<int32_t>(uint8{a} + uint8{b}));

*/

