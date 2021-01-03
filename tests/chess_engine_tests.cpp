#include <array>
#include <map>
#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleString.h"

// So we can check the value of private members
#define private public
#include "chess_engine.h"
#undef private

#include "test_utils.h"
#include "board_state.h"

TEST_GROUP(ChessEngine_Tests)
{
    using Idx = TileName;
    TileIndex d4 = Idx::D4;

    ChessEngine engine;
    BoardState bs;

    std::map<Direction, Bitboard> d4_rays;

    void setup() {
        d4_rays[Direction::North]     = Bitboard(Idx::D5) | Bitboard(Idx::D6) | Bitboard(Idx::D7) | Bitboard(Idx::D8);
        d4_rays[Direction::NorthEast] = Bitboard(Idx::E5) | Bitboard(Idx::F6) | Bitboard(Idx::G7) | Bitboard(Idx::H8);
        d4_rays[Direction::East]      = Bitboard(Idx::E4) | Bitboard(Idx::F4) | Bitboard(Idx::G4) | Bitboard(Idx::H4);
        d4_rays[Direction::SouthEast] = Bitboard(Idx::E3) | Bitboard(Idx::F2) | Bitboard(Idx::G1);
        d4_rays[Direction::South]     = Bitboard(Idx::D3) | Bitboard(Idx::D2) | Bitboard(Idx::D1);
        d4_rays[Direction::SouthWest] = Bitboard(Idx::C3) | Bitboard(Idx::B2) | Bitboard(Idx::A1);
        d4_rays[Direction::West]      = Bitboard(Idx::C4) | Bitboard(Idx::B4) | Bitboard(Idx::A4);
        d4_rays[Direction::NorthWest] = Bitboard(Idx::C5) | Bitboard(Idx::B6) | Bitboard(Idx::A7);
    }
    void teardown() {}
};

TEST(ChessEngine_Tests, GetEmptyBoardRayAttacks)
{
    std::vector<Direction> dirs { Direction::North, Direction::South, Direction::East, Direction::West,
        Direction::NorthEast, Direction::SouthEast, Direction::SouthWest, Direction::NorthWest};

    for (auto d: dirs)
        CHECK_EQUAL(d4_rays[d], engine.GetEmptyBoardRayAttacks(d4, d));
}

TEST(ChessEngine_Tests, GetRayAttacks)
{
    // Check all attack directions with no blockers
    engine.occupied_tiles_ = Bitboard(0);
    for (auto const& pair: d4_rays) {
        CHECK_EQUAL(pair.second, engine.GetRayAttacks(d4, pair.first));
    }

    // Pieces that block rays from D4, at various distances from the edge of the board.
    Bitboard edge_blockers = Bitboard(0).BitSet(Idx::D8).BitSet(Idx::H8).BitSet(Idx::H4).BitSet(Idx::G1)
                                        .BitSet(Idx::D1).BitSet(Idx::A1).BitSet(Idx::A4).BitSet(Idx::A7);
    Bitboard one_from_edge_blockers = Bitboard(0).BitSet(Idx::D7).BitSet(Idx::G7).BitSet(Idx::G4).BitSet(Idx::F2)
                                                 .BitSet(Idx::D2).BitSet(Idx::B2).BitSet(Idx::B4).BitSet(Idx::B6);
    Bitboard two_from_edge_blockers = Bitboard(0).BitSet(Idx::D6).BitSet(Idx::F6).BitSet(Idx::F4).BitSet(Idx::E3)
                                                 .BitSet(Idx::D3).BitSet(Idx::C3).BitSet(Idx::C4).BitSet(Idx::C5);

    engine.occupied_tiles_ = edge_blockers;
    for (auto const& pair: d4_rays) {
        CHECK_EQUAL(pair.second, engine.GetRayAttacks(d4, pair.first));
    }

    engine.occupied_tiles_ = one_from_edge_blockers;
    for (auto const& pair: d4_rays) {
        CHECK_EQUAL(pair.second & ~edge_blockers, engine.GetRayAttacks(d4, pair.first));
    }

    engine.occupied_tiles_ = two_from_edge_blockers;
    for (auto const& pair: d4_rays) {
        CHECK_EQUAL(pair.second & ~(edge_blockers | one_from_edge_blockers),
            engine.GetRayAttacks(d4, pair.first));
    }
}