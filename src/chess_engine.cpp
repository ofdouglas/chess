#include "chess_engine.h"
#include "bitboard.h"
#include <cstdlib>

#include <cstdio>

Move ChessEngine::SelectMove(BoardState& bs) {
    std::vector<Move> moves = GenerateMoves(bs);

    // for (int i = 0; i < moves.size(); i++) {
    //     moves[i].Print();
    // }

    return moves[rand() % moves.size()];
}


std::vector<Move> ChessEngine::GenerateMoves(BoardState& bs) {
    Bitboard targets = bs.GetOpponentBitboards().GetBitboardsUnion();
    Bitboard friendlies = bs.GetSelfBitboards().GetBitboardsUnion();

    //return GeneratePawnMoves(bs, targets, friendlies);
    return GenerateKnightMoves(bs, targets, friendlies);
}


std::vector<Move> ChessEngine::GenerateKnightMoves(BoardState& bs, Bitboard targets, Bitboard friendlies) {
    Bitboard knights = bs.GetSelfBitboards().knights;
    Bitboard empty_tiles = targets.LogicOr(friendlies).Invert();

    std::vector<Move> moves;
    Move move;
    move.piece_type = PIECE_TYPE_KNIGHT;

    while (knights.GetBits()) {
        unsigned knight_index = knights.BitscanForward();
        knights.BitClear(knight_index);
        move.src_tile_index = knight_index;

        int knight_rank = RankFromTileIndex(knight_index);
        int knight_file = FileFromTileIndex(knight_index);        
        int d4_rank = RankFromTileIndex(D4_TILE_INDEX);
        int d4_file = FileFromTileIndex(D4_TILE_INDEX);

        Bitboard attacks(KNIGHT_PATTERN_D4);
        attacks = attacks.Shift(knight_rank - d4_rank, knight_file - d4_file);

        printf("knight attacks = 0x%016lx\n", attacks.GetBits());

        Bitboard quiet_moves = attacks.LogicAnd(empty_tiles);
        attacks = attacks.LogicAnd(targets);

        move.captures = true;
        while (attacks.GetBits()) {
            move.dest_tile_index = attacks.BitscanForward();
            moves.push_back(move);
            attacks.BitClear(move.dest_tile_index);
        }

        move.captures = false;
        while (quiet_moves.GetBits()) {
            move.dest_tile_index = quiet_moves.BitscanForward();
            moves.push_back(move);
            quiet_moves.BitClear(move.dest_tile_index);
        }
    }

    return moves;
}


// Doesn't generate en-passant capture or promotion moves yet
//
std::vector<Move> ChessEngine::GeneratePawnMoves(BoardState& bs, Bitboard targets, Bitboard friendlies) {
    Bitboard pawns = bs.GetSelfBitboards().pawns;
    Bitboard empty_tiles = targets.LogicOr(friendlies).Invert();

    struct attacks_ {
        Bitboard bb;
        int offset;
        bool captures;
    } attacks[4];

    if (bs.GetPlayerToMove() == COLOR_WHITE) {
        attacks[0].bb = pawns.StepNorthWest().LogicAnd(targets);
        attacks[0].offset = NORTHWEST_OFFSET;
        attacks[0].captures = true;
        attacks[1].bb = pawns.StepNorthEast().LogicAnd(targets);
        attacks[1].offset = NORTHEAST_OFFSET;
        attacks[1].captures = true;
        attacks[2].bb = pawns.StepNorth().LogicAnd(empty_tiles);
        attacks[2].offset = NORTH_OFFSET;
        attacks[2].captures = false;
        attacks[3].bb = attacks[2].bb.StepNorth().LogicAnd(empty_tiles);
        attacks[3].offset = NORTH_OFFSET * 2;
        attacks[3].captures = false;
    } else {
        attacks[0].bb = pawns.StepSouthEast().LogicAnd(targets);
        attacks[0].offset = SOUTHEAST_OFFSET;
        attacks[0].captures = true;
        attacks[1].bb = pawns.StepSouthWest().LogicAnd(targets);
        attacks[1].offset = SOUTHWEST_OFFSET;
        attacks[1].captures = true;
        attacks[2].bb = pawns.StepSouth().LogicAnd(empty_tiles);
        attacks[2].offset = SOUTH_OFFSET;
        attacks[2].captures = false;
        attacks[3].bb = attacks[2].bb.StepSouth().LogicAnd(empty_tiles);
        attacks[3].offset = SOUTH_OFFSET * 2;
        attacks[3].captures = false;
    }

    std::vector<Move> moves;
    Move move;
    move.piece_type = PIECE_TYPE_PAWN;

    for (int i = 0; i < 4; i++) {
        while (attacks[i].bb.GetBits()) {
            // To add queen promotions: if move.dest_tile_index is in the 8th rank,
            // add two versions of this move to the list: one with a queen promotion,
            // the other with a knight promotion. Ignore rook and bishop promotions.
            move.dest_tile_index = attacks[i].bb.BitscanForward();
            move.src_tile_index = move.dest_tile_index - attacks[i].offset;
            move.captures = attacks[i].captures;
            moves.push_back(move);
            attacks[i].bb.BitClear(move.dest_tile_index);
        }
    }

    return moves;
}

