#include "board_state.h"


BoardState::BoardState()
{
    // Initialize pawns
    bitboards[COLOR_WHITE].pawns = INITIAL_WHITE_PAWNS;
    bitboards[COLOR_BLACK].pawns = INITIAL_WHITE_PAWNS << (8 * 5);

    // Initialize pieces
    for (int i = 0; i < 2; i++) {
        unsigned shift_amount = (i == COLOR_BLACK) ? 56 : 0UL;  // 7 ranks, 8 files each
        bitboards[i].knights = INITIAL_WHITE_KNIGHTS << shift_amount;
        bitboards[i].bishops = INITIAL_WHITE_BISHOPS << shift_amount;
        bitboards[i].rooks = INITIAL_WHITE_ROOKS << shift_amount;
        bitboards[i].queens = INITIAL_WHITE_QUEENS << shift_amount;
        bitboards[i].king = INITIAL_WHITE_KING << shift_amount;
    }
}

enum Color BoardState::GetPlayerToMove() const {
    // Black moves on odd plies, since the ply count starts at 0.
    return (ply_counter & 1) ? COLOR_BLACK : COLOR_WHITE;    
}

PlayerBitboards& BoardState::GetSelfBitboards() {
    return bitboards[GetPlayerToMove()];
}

PlayerBitboards& BoardState::GetOpponentBitboards() {
    return bitboards[GetPlayerToMove() ^ 1];
}

TileContents BoardState::GetTile(unsigned tile_index) const {
    TileContents tc;

    tc.piece_type = bitboards[COLOR_BLACK].GetTile(tile_index);
    if (tc.piece_type != PIECE_TYPE_NONE) {
        tc.color = COLOR_BLACK;
    } else {
        tc.piece_type = bitboards[COLOR_WHITE].GetTile(tile_index);
        tc.color = COLOR_WHITE;
    }

    return tc;
}
