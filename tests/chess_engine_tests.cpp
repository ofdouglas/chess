#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleString.h"
#include <cstdio>

// So we can check the value of private members
#define private public
#include "chess_engine.h"
#undef private

#include "test_utils.h"
#include "board_state.h"

TEST_GROUP(ChessEngine_Tests)
{
    ChessEngine engine;
    BoardState bs;

    void setup() {}
    void teardown() {}
};

TEST(ChessEngine_Tests, GetEmptyBoardRayAttacks)
{
    using Idx = TileIndex::TileName;
    TileIndex d4(Idx::D4);

    Bitboard north_ray
        {Bitboard(Idx::D5) | Bitboard(Idx::D6) | Bitboard(Idx::D7) | Bitboard(Idx::D8)};
    Bitboard northeast_ray
        {Bitboard(Idx::E5) | Bitboard(Idx::F6) | Bitboard(Idx::G7) | Bitboard(Idx::H8)};
    Bitboard east_ray
        {Bitboard(Idx::E4) | Bitboard(Idx::F4) | Bitboard(Idx::G4) | Bitboard(Idx::H4)};
    Bitboard southeast_ray
        {Bitboard(Idx::E3) | Bitboard(Idx::F2) | Bitboard(Idx::G1)};
    Bitboard south_ray
        {Bitboard(Idx::D3) | Bitboard(Idx::D2) | Bitboard(Idx::D1)};
    Bitboard southwest_ray
        {Bitboard(Idx::C3) | Bitboard(Idx::B2) | Bitboard(Idx::A1)};
    Bitboard west_ray
        {Bitboard(Idx::C4) | Bitboard(Idx::B4) | Bitboard(Idx::A4)};
    Bitboard northwest_ray
        {Bitboard(Idx::C5) | Bitboard(Idx::B6) | Bitboard(Idx::A7)};

    CHECK_EQUAL(north_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::North));
    CHECK_EQUAL(northeast_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::NorthEast));
    CHECK_EQUAL(east_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::East));
    CHECK_EQUAL(southeast_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::SouthEast));
    CHECK_EQUAL(south_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::South));
    CHECK_EQUAL(southwest_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::SouthWest));
    CHECK_EQUAL(west_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::West));
    CHECK_EQUAL(northwest_ray, engine.GetEmptyBoardRayAttacks(d4, Direction::NorthWest));
}
