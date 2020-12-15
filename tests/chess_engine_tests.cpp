#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleString.h"
#include <cstdio>

// So we can check the value of private members
#define private public
#include "chess_engine.h"
#undef private

#include "bitboard.h"
#include "board_state.h"

TEST_GROUP(ChessEngine_Tests)
{
    ChessEngine engine;
    BoardState bs;

    void setup() {}
    void teardown() {}
};

TEST(ChessEngine_Tests, GeneratePawnMoves)
{
    



}
