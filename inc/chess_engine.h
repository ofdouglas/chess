#ifndef CHESS_ENGINE_H_DEFINED
#define CHESS_ENGINE_H_DEFINED

#include "chess_common.h"
#include "board_state.h"
#include <vector>

class ChessEngine {
public:
    ChessEngine() {}

    Move SelectMove(BoardState& bs);

    // Generates all legal moves for the current position and returns true
    // if the given move matches one of them.
    // -- Useful for validating human player inputs and for testing purposes, 
    // but very slow -- the CPU player should never call this function.
    bool IsLegalMove(BoardState& bs, Move move);

    bool IsOwnKingInCheck(BoardState& bs);

private:

    void GenerateMoves(BoardState& bs);
    void GeneratePawnMoves(BoardState& bs);
    void GenerateKnightMoves(BoardState& bs);
    void GenerateBishopMoves(BoardState& bs);
    void GenerateRookMoves(BoardState& bs);
    void GenerateQueenMoves(BoardState& bs);
    void GenerateKingMoves(BoardState& bs);

    Bitboard GetEmptyBoardRayAttacks(unsigned tile_index, Direction dir) const;
    Bitboard GetRayAttacks(unsigned tile_index, Direction dir) const;
    Bitboard GetRookAttacks(unsigned tile_index) const;
    Bitboard GetBishopAttacks(unsigned tile_index) const;
    Bitboard GetQueenAttacks(unsigned tile_index) const;
    Bitboard GetKnightAttacks(unsigned tile_index) const;
    Bitboard GetKingAttacks(unsigned tile_index) const;

    // Initialized each time GenerateMoves is called
    std::vector<Move> move_list_;
    Bitboard targets_;
    Bitboard friendlies_;
    Bitboard empty_tiles_;
    Bitboard occupied_tiles_;
};




#endif // CHESS_ENGINE_H_DEFINED