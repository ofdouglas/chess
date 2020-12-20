#include "board_state.h"
#include <cstring>

BoardState::BoardState() {
    // Initialize pawns
    bitboards[static_cast<int>(Color::White)].pawns = Bitboard::initial_white_pawn_bits;
    bitboards[static_cast<int>(Color::Black)].pawns = Bitboard::initial_white_pawn_bits << (8 * 5);

    // Initialize pieces
    for (int i = 0; i < 2; i++) {
        unsigned shift_amount = (i == static_cast<int>(Color::Black)) ? 56 : 0UL;  // 7 ranks, 8 files each
        bitboards[i].knights = Bitboard::initial_white_knight_bits << shift_amount;
        bitboards[i].bishops = Bitboard::initial_white_bishop_bits << shift_amount;
        bitboards[i].rooks = Bitboard::initial_white_rook_bits << shift_amount;
        bitboards[i].queens = Bitboard::initial_white_queen_bits << shift_amount;
        bitboards[i].king = Bitboard::initial_white_king_bits << shift_amount;
    }
}

BoardState::BoardState(std::string init_state_fen) {
    // Not implemented yet
    assert(init_state_fen == "");

    memset(this, 0, sizeof(BoardState));
}

Color BoardState::GetPlayerToMove() const {
    // Black moves on odd plies, since the ply count starts at 0.
    return (ply_counter & 1) ? Color::Black : Color::White;    
}

PlayerBitboards& BoardState::GetSelfBitboards() {
    return bitboards[static_cast<int>(GetPlayerToMove())];
}

PlayerBitboards& BoardState::GetOpponentBitboards() {
    return bitboards[static_cast<int>(GetPlayerToMove()) ^ 1];
}

TileContents BoardState::GetTile(TileIndex index) const {
    TileContents tc;

    tc.piece_type = bitboards[static_cast<int>(Color::Black)].GetTile(index);
    if (tc.piece_type != PieceType::None) {
        tc.color = Color::Black;
    } else {
        tc.piece_type = bitboards[static_cast<int>(Color::White)].GetTile(index);
        tc.color = Color::White;
    }

    return tc;
}

bool BoardState::ApplyMove(Move move) {
    if (move.captures) {
        TileContents tc = GetTile(move.dest_tile_index);
        move.captured_type = tc.piece_type;
        GetOpponentBitboards().DeletePiece(move.dest_tile_index);
    }

    GetSelfBitboards().MovePiece(move);

    ply_counter++;

    // TODO: update castling rights, move counters, en passant info...

    return true;
}

void BoardState::SetTile(TileIndex index, TileContents tc) {
    Bitboard bb;
    if (tc.color == Color::White) {
        bb = bitboards[static_cast<int>(Color::White)].GetBitboardByType(tc.piece_type);
    } else {
        bb = bitboards[static_cast<int>(Color::Black)].GetBitboardByType(tc.piece_type);
    }

    bb.BitSet(index.ToInt());
}

// Possible enhancements to the evaluation:
//  * Account for doubled, blocked, and isolated pawns
//  * Account for mobility (total number of legal moves for the player)
//  * Account for location (piece value squares)
double BoardState::GetPlayerEvaluation(const PlayerBitboards& pb) const {
    constexpr double kPieceWeightPawn = 1.0;
    constexpr double kPieceWeightKnight = 3.0;
    constexpr double kPieceWeightBishop = 3.1;
    constexpr double kPieceWeightRook = 5.2;
    constexpr double kPieceWeightQueen = 9.0;
    constexpr double kPieceWeightKing = 20.0;

    double score = 0;
    score += __builtin_popcountll(pb.pawns.GetBits()) * kPieceWeightPawn;
    score += __builtin_popcountll(pb.knights.GetBits()) * kPieceWeightKnight;
    score += __builtin_popcountll(pb.bishops.GetBits()) * kPieceWeightBishop;
    score += __builtin_popcountll(pb.rooks.GetBits()) * kPieceWeightRook;
    score += __builtin_popcountll(pb.queens.GetBits()) * kPieceWeightQueen;
    score += __builtin_popcountll(pb.king.GetBits()) * kPieceWeightKing;
    return score;
}

double BoardState::GetEvaluation() const {
    return GetPlayerEvaluation(bitboards[static_cast<int>(Color::White)])
        - GetPlayerEvaluation(bitboards[static_cast<int>(Color::Black)]);
}