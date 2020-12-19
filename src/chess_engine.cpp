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
Bitboard ChessEngine::GetEmptyBoardRayAttacks(unsigned tile_index, Direction dir) const {
    Bitboard b((uint64_t)1 << tile_index);

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
    return b.BitClear(tile_index);
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

Bitboard ChessEngine::GetRayAttacks(unsigned tile_index, Direction dir) const {
    Bitboard attacks = GetEmptyBoardRayAttacks(tile_index, dir);
    Bitboard blockers = attacks & occupied_tiles_;

    // Reset all bits in attacks which are after the first blocker
    if (blockers.GetBits()) {
        unsigned blocker_index = blockers.Bitscan(IsNegative(dir) ? 
            Bitboard::BitscanDirection::Reverse : Bitboard::BitscanDirection::Forward);
        attacks = attacks ^ GetEmptyBoardRayAttacks(blocker_index, dir);
    }

    return attacks;
}

Bitboard ChessEngine::GetRookAttacks(unsigned tile_index) const {
    Bitboard attacks = GetRayAttacks(tile_index, Direction::North);
    attacks |= GetRayAttacks(tile_index, Direction::South);
    attacks |= GetRayAttacks(tile_index, Direction::East);
    attacks |= GetRayAttacks(tile_index, Direction::West);

    // Handles cases where the first blocker was a friendly
    return attacks & ~friendlies_;
}

Bitboard ChessEngine::GetBishopAttacks(unsigned tile_index) const {
    Bitboard attacks = GetRayAttacks(tile_index, Direction::NorthEast);
    attacks |= GetRayAttacks(tile_index, Direction::NorthWest);
    attacks |= GetRayAttacks(tile_index, Direction::SouthEast);
    attacks |= GetRayAttacks(tile_index, Direction::SouthWest);

    // Handles cases where the first blocker was a friendly
    return attacks & ~friendlies_;
}

Bitboard ChessEngine::GetQueenAttacks(unsigned tile_index) const {
    return GetRookAttacks(tile_index) | GetBishopAttacks(tile_index);
}

Bitboard ChessEngine::GetKnightAttacks(unsigned tile_index) const {
    int knight_rank = RankFromTileIndex(tile_index);
    int knight_file = FileFromTileIndex(tile_index);        
    int d4_rank = RankFromTileIndex(static_cast<int>(TileIndex::D4));
    int d4_file = FileFromTileIndex(static_cast<int>(TileIndex::D4));

    Bitboard attacks(Bitboard::knight_pattern_d4);
    return attacks.Shift(knight_rank - d4_rank, knight_file - d4_file);    
}

// Pseudo-legal, doesn't care if the attack would place the king in check.
// Doesn't generate castling moves.
Bitboard ChessEngine::GetKingAttacks(unsigned tile_index) const {
    Bitboard attacks;
    Bitboard king;

    king.BitSet(tile_index);
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
    Move move;
    move.piece_type = PieceType::Bishop;

    while (bishops.GetBits()) {
        unsigned bishop_index = bishops.BitscanForward();
        bishops.BitClear(bishop_index);
        move.src_tile_index = bishop_index;

        Bitboard attacks = GetBishopAttacks(bishop_index);
        Bitboard quiet_moves = attacks & empty_tiles_;
        attacks &= targets_;

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
}

void ChessEngine::GenerateRookMoves(BoardState& bs) {
    Bitboard rooks = bs.GetSelfBitboards().rooks;
    Move move;
    move.piece_type = PieceType::Rook;

    while (rooks.GetBits()) {
        unsigned rook_index = rooks.BitscanForward();
        rooks.BitClear(rook_index);
        move.src_tile_index = rook_index;

        Bitboard attacks = GetRookAttacks(rook_index);
        Bitboard quiet_moves = attacks &empty_tiles_;
        attacks &= targets_;

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
}

void ChessEngine::GenerateQueenMoves(BoardState& bs) {
    Bitboard queens = bs.GetSelfBitboards().queens;
    Move move;
    move.piece_type = PieceType::Queen;

    while (queens.GetBits()) {
        unsigned queen_index = queens.BitscanForward();
        queens.BitClear(queen_index);
        move.src_tile_index = queen_index;

        Bitboard attacks = GetRookAttacks(queen_index) | GetBishopAttacks(queen_index);
        Bitboard quiet_moves = attacks & empty_tiles_;
        attacks &= targets_;

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
}

void ChessEngine::GenerateKnightMoves(BoardState& bs) {
    Bitboard knights = bs.GetSelfBitboards().knights;
    Move move;
    move.piece_type = PieceType::Knight;

    while (knights.GetBits()) {
        unsigned knight_index = knights.BitscanForward();
        knights.BitClear(knight_index);
        move.src_tile_index = knight_index;

        Bitboard attacks = GetKnightAttacks(knight_index);
        Bitboard quiet_moves = attacks & empty_tiles_;
        attacks &= targets_;

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

    Move move;
    move.piece_type = PieceType::King;
    move.src_tile_index = king_index;

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
