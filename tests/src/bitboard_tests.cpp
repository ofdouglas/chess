#include "bitboard.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleString.h"

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
