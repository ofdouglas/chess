#include "chess_engine.h"
#include "bitboard.h"
#include <cstdlib>

#include <cstdio>




Move ChessEngine::SelectMove(BoardState& bs) {
    GenerateMoves(bs);
    assert(move_list_.size() > 0);

    // for (int i = 0; i < move_list_.size(); i++) {
    //     move_list_[i].Print();
    // }

    return move_list_[rand() % move_list_.size()];
}

// TODO: detect checks against our king
bool ChessEngine::IsLegalMove(BoardState& bs, Move move) {
    GenerateMoves(bs);

    std::vector<Move>::iterator it;
    for (it = move_list_.begin(); it != move_list_.end(); it++) {
        if (it->src_tile_index == move.src_tile_index &&
            it->dest_tile_index == move.dest_tile_index) {
            return true;
        }
    }
    return false;
}

bool ChessEngine::IsOwnKingInCheck(BoardState& bs) {
    unsigned king_index = bs.GetSelfBitboards().king.BitscanForward();

    Bitboard knight_attacks = GetKnightAttacks(king_index);
    Bitboard bishop_attacks = GetBishopAttacks(king_index);
    Bitboard rook_attacks = GetRookAttacks(king_index);
    
    // TODO: implement
    return false;
}

void ChessEngine::GenerateMoves(BoardState& bs) {
    targets_ = bs.GetOpponentBitboards().GetBitboardsUnion();
    friendlies_ = bs.GetSelfBitboards().GetBitboardsUnion();
    occupied_tiles_ = targets_ | friendlies_;
    empty_tiles_ = ~occupied_tiles_;

    move_list_.clear();

    GeneratePawnMoves(bs);
    GenerateKnightMoves(bs);
    GenerateBishopMoves(bs);
    GenerateRookMoves(bs);
    GenerateQueenMoves(bs);
    GenerateKingMoves(bs);
}

// TODO: replace with table lookup. Can use this to generate the tables.
Bitboard ChessEngine::GetEmptyBoardRayAttacks(TileIndex index, Direction dir) const {
    Bitboard b(index);

    int shifts[2] = {};
    switch (dir) {
        case Direction::North:       shifts[0] =  1; shifts[1] =  0;   break;
        case Direction::South:       shifts[0] = -1; shifts[1] =  0;   break;
        case Direction::East:        shifts[0] =  0; shifts[1] =  1;   break;
        case Direction::West:        shifts[0] =  0; shifts[1] = -1;   break;
        case Direction::NorthEast:   shifts[0] =  1; shifts[1] =  1;   break;
        case Direction::NorthWest:   shifts[0] =  1; shifts[1] = -1;   break;
        case Direction::SouthEast:   shifts[0] = -1; shifts[1] =  1;   break;
        case Direction::SouthWest:   shifts[0] = -1; shifts[1] = -1;   break;
    }

    for (int i = 1; i <= 7; i++) {
        b |= b.Shift(shifts[0], shifts[1]);
    }
    return b.BitClear(index);
}

// A negative attack direction is one for which we use BitscanDirection::Reverse instead of 
// BitscanDirection::Forward when finding the first blocker (in the context of ray attack generation).
bool IsNegative(Direction dir)  {
    switch (dir) {
        case Direction::North:       return false;
        case Direction::South:       return true;
        case Direction::East:        return false;
        case Direction::West:        return true;
        case Direction::NorthEast:   return false;
        case Direction::NorthWest:   return false;
        case Direction::SouthEast:   return true;
        case Direction::SouthWest:   return true;
    }
    assert(0);
    return false;
}

Bitboard ChessEngine::GetRayAttacks(TileIndex index, Direction dir) const {
    Bitboard attacks = GetEmptyBoardRayAttacks(index, dir);
    Bitboard blockers = attacks & occupied_tiles_;

    // Reset all bits in attacks which are after the first blocker
    if (blockers.GetBits()) {
        unsigned blocker_index = blockers.Bitscan(IsNegative(dir) ? 
            Bitboard::BitscanDirection::Reverse : Bitboard::BitscanDirection::Forward);
        attacks = attacks ^ GetEmptyBoardRayAttacks(blocker_index, dir);
    }

    return attacks;
}

Bitboard ChessEngine::GetRookAttacks(TileIndex index) const {
    Bitboard attacks = GetRayAttacks(index, Direction::North);
    attacks |= GetRayAttacks(index, Direction::South);
    attacks |= GetRayAttacks(index, Direction::East);
    attacks |= GetRayAttacks(index, Direction::West);

    // Handles cases where the first blocker was a friendly
    return attacks & ~friendlies_;
}

Bitboard ChessEngine::GetBishopAttacks(TileIndex index) const {
    Bitboard attacks = GetRayAttacks(index, Direction::NorthEast);
    attacks |= GetRayAttacks(index, Direction::NorthWest);
    attacks |= GetRayAttacks(index, Direction::SouthEast);
    attacks |= GetRayAttacks(index, Direction::SouthWest);

    // Handles cases where the first blocker was a friendly
    return attacks & ~friendlies_;
}

Bitboard ChessEngine::GetQueenAttacks(TileIndex index) const {
    return GetRookAttacks(index) | GetBishopAttacks(index);
}

Bitboard ChessEngine::GetKnightAttacks(TileIndex index) const {
    int knight_rank = index.Rank();
    int knight_file = index.File();
    int d4_rank = TileIndex(TileName::D4).Rank();
    int d4_file = TileIndex(TileName::D4).File();

    Bitboard attacks(Bitboard::knight_pattern_d4);
    return attacks.Shift(knight_rank - d4_rank, knight_file - d4_file);    
}

// Pseudo-legal, doesn't care if the attack would place the king in check.
// Doesn't generate castling moves.
Bitboard ChessEngine::GetKingAttacks(TileIndex index) const {
    Bitboard attacks;
    Bitboard king;

    king.BitSet(index);
    attacks |= king.StepNorth();
    attacks |= king.StepSouth();
    attacks |= king.StepEast();
    attacks |= king.StepWest();
    attacks |= king.StepNorthEast();
    attacks |= king.StepNorthWest();
    attacks |= king.StepSouthEast();
    attacks |= king.StepSouthWest();

    return attacks;
}


void ChessEngine::GenerateBishopMoves(BoardState& bs) {
    Bitboard bishops = bs.GetSelfBitboards().bishops;

    while (bishops.GetBits()) {
        unsigned bishop_index = bishops.BitscanForward();
        bishops.BitClear(bishop_index);

        Bitboard attacks = GetBishopAttacks(bishop_index);
        Bitboard quiet_moves = attacks & empty_tiles_;
        attacks &= targets_;

        EnqueueMoves(bs, PieceType::Bishop, TileIndex(bishop_index), attacks, quiet_moves);    
    }
}

void ChessEngine::GenerateRookMoves(BoardState& bs) {
    Bitboard rooks = bs.GetSelfBitboards().rooks;

    while (rooks.GetBits()) {
        unsigned rook_index = rooks.BitscanForward();
        rooks.BitClear(rook_index);

        Bitboard attacks = GetRookAttacks(rook_index);
        Bitboard quiet_moves = attacks &empty_tiles_;
        attacks &= targets_;

        EnqueueMoves(bs, PieceType::Rook, TileIndex(rook_index), attacks, quiet_moves);     
    }
}

void ChessEngine::GenerateQueenMoves(BoardState& bs) {
    Bitboard queens = bs.GetSelfBitboards().queens;

    while (queens.GetBits()) {
        unsigned queen_index = queens.BitscanForward();
        queens.BitClear(queen_index);

        Bitboard attacks = GetRookAttacks(queen_index) | GetBishopAttacks(queen_index);
        Bitboard quiet_moves = attacks & empty_tiles_;
        attacks &= targets_;

        EnqueueMoves(bs, PieceType::Queen, TileIndex(queen_index), attacks, quiet_moves);   
    }
}

void ChessEngine::GenerateKnightMoves(BoardState& bs) {
    Bitboard knights = bs.GetSelfBitboards().knights;

    while (knights.GetBits()) {
        unsigned knight_index = knights.BitscanForward();
        knights.BitClear(knight_index);

        Bitboard attacks = GetKnightAttacks(knight_index);
        Bitboard quiet_moves = attacks & empty_tiles_;
        attacks &= targets_;

        EnqueueMoves(bs, PieceType::Knight, TileIndex(knight_index), attacks, quiet_moves);
    }
}


// Doesn't generate en-passant capture or promotion moves yet
//
void ChessEngine::GeneratePawnMoves(BoardState& bs) {
    Bitboard pawns = bs.GetSelfBitboards().pawns;

    struct attacks_ {
        Bitboard bb;
        int offset;
        bool captures;
    } attacks[4];

    if (bs.GetPlayerToMove() == Color::White) {
        attacks[0].bb = pawns.StepNorthWest() & targets_;
        attacks[0].offset = TileIndexOffsetFromDirection(Direction::NorthWest);
        attacks[0].captures = true;
        attacks[1].bb = pawns.StepNorthEast() & targets_;
        attacks[1].offset = TileIndexOffsetFromDirection(Direction::NorthEast);
        attacks[1].captures = true;
        attacks[2].bb = pawns.StepNorth() & empty_tiles_;
        attacks[2].offset = TileIndexOffsetFromDirection(Direction::North);
        attacks[2].captures = false;
        attacks[3].bb = attacks[2].bb.StepNorth() & empty_tiles_ & Bitboard(Bitboard::rank_4_bits);
        attacks[3].offset = TileIndexOffsetFromDirection(Direction::North) * 2;
        attacks[3].captures = false;
    } else {
        attacks[0].bb = pawns.StepSouthEast() & targets_;
        attacks[0].offset = TileIndexOffsetFromDirection(Direction::SouthEast);
        attacks[0].captures = true;
        attacks[1].bb = pawns.StepSouthWest() & targets_;
        attacks[1].offset = TileIndexOffsetFromDirection(Direction::SouthWest);
        attacks[1].captures = true;
        attacks[2].bb = pawns.StepSouth() & empty_tiles_;
        attacks[2].offset = TileIndexOffsetFromDirection(Direction::South);
        attacks[2].captures = false;
        attacks[3].bb = attacks[2].bb.StepSouth() & empty_tiles_ & Bitboard(Bitboard::rank_5_bits);
        attacks[3].offset = TileIndexOffsetFromDirection(Direction::South) * 2;
        attacks[3].captures = false;
    }

    Move move;
    move.piece_type = PieceType::Pawn;

    for (int i = 0; i < 4; i++) {
        while (attacks[i].bb.GetBits()) {
            // To add queen promotions: if move.dest_tile_index is in the 8th rank,
            // add two versions of this move to the list: one with a queen promotion,
            // the other with a knight promotion. Ignore rook and bishop promotions.
            move.dest_tile_index = attacks[i].bb.BitscanForward();
            move.src_tile_index = move.dest_tile_index - attacks[i].offset;
            move.captures = attacks[i].captures;
            move_list_.push_back(move);
            attacks[i].bb.BitClear(move.dest_tile_index);
        }
    }
}

void ChessEngine::GenerateKingMoves(BoardState& bs) {
    unsigned king_index = bs.GetSelfBitboards().king.BitscanForward();
    Bitboard attacks = GetKingAttacks(king_index);
    Bitboard quiet_moves = attacks & empty_tiles_;
    attacks &= targets_;
    EnqueueMoves(bs, PieceType::King, king_index, attacks, quiet_moves);
}

void ChessEngine::EnqueueMoves(BoardState&bs, PieceType type, TileIndex source, 
        Bitboard attacks, Bitboard quiet_moves) {

    Move move;
    move.piece_type = type;
    move.src_tile_index = source;

    move.captures = true;
    while (attacks.GetBits()) {
        move.dest_tile_index = attacks.BitscanForward();
        move_list_.push_back(move);
        attacks.BitClear(move.dest_tile_index);
    }

    move.captures = false;
    while (quiet_moves.GetBits()) {
        move.dest_tile_index = quiet_moves.BitscanForward();
        move_list_.push_back(move);
        quiet_moves.BitClear(move.dest_tile_index);
    }    
}
