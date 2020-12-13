#ifndef CHESS_ENGINE_H_DEFINED
#define CHESS_ENGINE_H_DEFINED

#include "chess_common.h"
#include "board_state.h"
#include <vector>

class ChessEngine {
public:
    ChessEngine() {}

    Move SelectMove(BoardState& bs);

private:

    std::vector<Move> GenerateMoves(BoardState& bs);
    std::vector<Move> GeneratePawnMoves(BoardState& bs, Bitboard targets, Bitboard friendlies);
    std::vector<Move> GenerateKnightMoves(BoardState& bs, Bitboard targets, Bitboard friendlies);
    std::vector<Move> GenerateBishopMoves(BoardState& bs, Bitboard targets, Bitboard friendlies);
    std::vector<Move> GenerateRookMoves(BoardState& bs, Bitboard targets, Bitboard friendlies);
    std::vector<Move> GenerateQueenMoves(BoardState& bs, Bitboard targets, Bitboard friendlies);
    std::vector<Move> GenerateKingMoves(BoardState& bs, Bitboard targets, Bitboard friendlies);
};


#endif // CHESS_ENGINE_H_DEFINED