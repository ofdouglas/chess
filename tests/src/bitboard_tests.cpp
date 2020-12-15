#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleString.h"
#include <cstdio>

// So we can check the value of private members
#define private public
#include "bitboard.h"
#undef private

SimpleString StringFrom(Bitboard b) {
    return StringFromFormat("%016lx", b.GetBits());
}

TEST_GROUP(Bitboard_Tests)
{
    void Setup() {}
    void Teardown() {}
};

Bitboard one_bit_set(int rank, int file)
{
    int tile_index = rank * 8 + file;
    return Bitboard((uint64_t)1 << tile_index);
}

TEST(Bitboard_Tests, Constructors)
{
    Bitboard empty = Bitboard();
    Bitboard nonempty = Bitboard(123);

    CHECK_EQUAL(0, empty.bits_);
    CHECK_EQUAL(123, nonempty.bits_);
}

TEST(Bitboard_Tests, GetBits)
{
    Bitboard b = Bitboard(123);
    CHECK_EQUAL(b.bits_, b.GetBits());
}

TEST(Bitboard_Tests, BitSet)
{
    Bitboard b = Bitboard();
    uint64_t x = 0;

    for (int i = 0; i < 64; i++) {
        b.BitSet(i);
        x |= (uint64_t)1 << i;

        CHECK_EQUAL(x, b.bits_);

        // Setting an already set bit should do nothing
        b.BitSet(i);
        CHECK_EQUAL(x, b.bits_);
    }
}

TEST(Bitboard_Tests, BitClear)
{
    Bitboard b = ~Bitboard(0);
    uint64_t x = ~0ULL;

    for (int i = 0; i < 64; i++) {
        b.BitClear(i);
        x &= ~((uint64_t)1 << i);

        CHECK_EQUAL(x, b.bits_);

        // Clearing an already clear bit should do nothing
        b.BitClear(i);
        CHECK_EQUAL(x, b.bits_);
    }
}

TEST(Bitboard_Tests, BitTest)
{
    Bitboard one = Bitboard(0xAAAAAAAAAAAAAAAA);
    Bitboard two = ~one;

    for (int i = 0; i < 64; i++) {
        if (i & 1) {
            CHECK(one.BitTest(i));
            CHECK_FALSE(two.BitTest(i));
        } else {
            CHECK(two.BitTest(i));
            CHECK_FALSE(one.BitTest(i));
        }
    }
}

TEST(Bitboard_Tests, BitscanForward)
{
    Bitboard b = ~Bitboard(0);

    for (int i = 0; i < 64; i++) {
        unsigned index = b.BitscanForward();
        CHECK_EQUAL(i, index);
        b.BitClear(i);
    }
}

TEST(Bitboard_Tests, BitscanReverse)
{
    Bitboard b = ~Bitboard(0);

    for (int i = 63; i >= 0; i--) {
        unsigned index = b.BitscanReverse();
        CHECK_EQUAL(i, index);
        b.BitClear(i);
    }
}

TEST(Bitboard_Tests, Bitscan)
{
    Bitboard b = ~Bitboard(0);

    for (int i = 0; i < 64; i++) {
        unsigned index = b.Bitscan(Bitboard::BITSCAN_FORWARD);
        CHECK_EQUAL(i, index);
        b.BitClear(i);
    }

    b = ~Bitboard(0);
    for (int i = 63; i >= 0; i--) {
        unsigned index = b.Bitscan(Bitboard::BITSCAN_REVERSE);
        CHECK_EQUAL(i, index);
        b.BitClear(i);
    }
}

TEST(Bitboard_Tests, BitwiseOr)
{
    Bitboard a = Bitboard(0xAAAAAAAAAAAAAAAA);
    Bitboard b = ~a;

    CHECK_EQUAL(a, a | a);
    CHECK_EQUAL(a, a | 0);
    CHECK_EQUAL(~Bitboard(0), a | b);

    Bitboard c = Bitboard(0);
    c |= a;
    CHECK_EQUAL(a, c);
    c |= a;
    CHECK_EQUAL(a, c);
}

TEST(Bitboard_Tests, BitwiseAnd)
{
    Bitboard a = Bitboard(0xAAAAAAAAAAAAAAAA);
    Bitboard b = ~b;

    CHECK_EQUAL(a, a & a);
    CHECK_EQUAL(Bitboard(0), a & 0);
    CHECK_EQUAL(a, ~Bitboard(0) & a);

    Bitboard c = ~Bitboard(0);
    c &= a;
    CHECK_EQUAL(a, c);
    c &= Bitboard(0);
    CHECK_EQUAL(Bitboard(0), c);
}

TEST(Bitboard_Tests, BitwiseXor)
{
    Bitboard a = Bitboard(0xAAAAAAAAAAAAAAAA);
    Bitboard b = ~a;

    CHECK_EQUAL(Bitboard(0), a ^ a);
    CHECK_EQUAL(a, a ^ 0);
    CHECK_EQUAL(~Bitboard(0), a ^ b);

    Bitboard c = Bitboard(0);
    c ^= a;
    CHECK_EQUAL(a, c);
    c ^= a;
    CHECK_EQUAL(Bitboard(0), c);
}

TEST(Bitboard_Tests, BitwiseNot)
{
    Bitboard zeros = Bitboard(0);
    Bitboard ones = Bitboard(~0ULL);

    CHECK_EQUAL(ones, ~zeros);
    CHECK_EQUAL(zeros, ~ones);
    CHECK_EQUAL(ones, ~(~ones));
    CHECK_EQUAL(zeros, ~(~zeros));
}

TEST(Bitboard_Tests, EqualityInequality)
{
    Bitboard zeros = Bitboard(0);
    Bitboard ones = Bitboard(~0ULL);
    Bitboard ones_copy = ones;

    CHECK(zeros != ones);
    CHECK_FALSE(zeros == ones);
    CHECK(ones_copy == ones);
    CHECK_FALSE(ones_copy != ones);
}

TEST(Bitboard_Tests, BitboardStep)
{
    // Stepping in any direction near the center of the board
    Bitboard b = one_bit_set(3, 3);
    CHECK_EQUAL(one_bit_set(4, 3), b.StepNorth());
    CHECK_EQUAL(one_bit_set(2, 3), b.StepSouth());
    CHECK_EQUAL(one_bit_set(3, 4), b.StepEast());
    CHECK_EQUAL(one_bit_set(3, 2), b.StepWest());
    CHECK_EQUAL(one_bit_set(4, 4), b.StepNorthEast());
    CHECK_EQUAL(one_bit_set(4, 2), b.StepNorthWest());
    CHECK_EQUAL(one_bit_set(2, 4), b.StepSouthEast());
    CHECK_EQUAL(one_bit_set(2, 2), b.StepSouthWest());

    // Bits shifted off the A and H files disappear instead of wrapping
    CHECK_EQUAL(Bitboard(0), one_bit_set(3, 0).StepWest());
    CHECK_EQUAL(Bitboard(0), one_bit_set(3, 0).StepNorthWest());
    CHECK_EQUAL(Bitboard(0), one_bit_set(3, 0).StepSouthWest());    

    CHECK_EQUAL(Bitboard(0), one_bit_set(3, 7).StepEast());
    CHECK_EQUAL(Bitboard(0), one_bit_set(3, 7).StepNorthEast());
    CHECK_EQUAL(Bitboard(0), one_bit_set(3, 7).StepSouthEast());

    // Bits shifted off the 1 and 8 ranks disappear instead of wrapping
    CHECK_EQUAL(Bitboard(0), one_bit_set(7, 3).StepNorth());
    CHECK_EQUAL(Bitboard(0), one_bit_set(7, 3).StepNorthEast());
    CHECK_EQUAL(Bitboard(0), one_bit_set(7, 3).StepNorthWest());

    CHECK_EQUAL(Bitboard(0), one_bit_set(0, 3).StepSouth());
    CHECK_EQUAL(Bitboard(0), one_bit_set(0, 3).StepSouthEast());
    CHECK_EQUAL(Bitboard(0), one_bit_set(0, 3).StepSouthWest());
}

TEST(Bitboard_Tests, BitboardShift)
{
    Bitboard b = one_bit_set(3, 3);

    // Shifting from the center to each corner
    CHECK_EQUAL(one_bit_set(0, 0), b.Shift(-3, -3));
    CHECK_EQUAL(one_bit_set(0, 7), b.Shift(-3, 4));
    CHECK_EQUAL(one_bit_set(7, 0), b.Shift(4, -3));
    CHECK_EQUAL(one_bit_set(7, 7), b.Shift(4, 4));

    // Shifting off the edge of the board
    CHECK_EQUAL(Bitboard(0), b.Shift(-4, 0));
    CHECK_EQUAL(Bitboard(0), b.Shift(5, 0));
    CHECK_EQUAL(Bitboard(0), b.Shift(0, -4));
    CHECK_EQUAL(Bitboard(0), b.Shift(0, 5));
}

