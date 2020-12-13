#include "bitboard.h"

/******************************************************************************
 * Player Bitboards Class
 *****************************************************************************/

PieceType PlayerBitboards::GetTile(unsigned tile_index) const {
    if (pawns.BitTest(tile_index))       return PIECE_TYPE_PAWN;
    if (knights.BitTest(tile_index))     return PIECE_TYPE_KNIGHT;
    if (bishops.BitTest(tile_index))     return PIECE_TYPE_BISHOP;
    if (rooks.BitTest(tile_index))       return PIECE_TYPE_ROOK;
    if (queens.BitTest(tile_index))      return PIECE_TYPE_QUEEN;
    if (king.BitTest(tile_index))        return PIECE_TYPE_KING;

    return PIECE_TYPE_NONE;
}

Bitboard PlayerBitboards::GetBitboardsUnion() const {
    uint64_t bb_union = pawns.GetBits();
    bb_union |= knights.GetBits();
    bb_union |= bishops.GetBits();
    bb_union |= rooks.GetBits();
    bb_union |= queens.GetBits();
    bb_union |= king.GetBits();

    return Bitboard(bb_union);

}

Bitboard& PlayerBitboards::GetBitboardByType(enum PieceType type) {
    switch (type){
        case PIECE_TYPE_PAWN:       return pawns;        break;
        case PIECE_TYPE_KNIGHT:     return knights;      break;
        case PIECE_TYPE_BISHOP:     return bishops;      break;
        case PIECE_TYPE_ROOK:       return rooks;        break;
        case PIECE_TYPE_QUEEN:      return queens;       break;
        case PIECE_TYPE_KING:       return king;         break;
    }
    assert(0);
    return pawns;
}

void PlayerBitboards::MovePiece(Move mv) {
    Bitboard& bb = GetBitboardByType(mv.piece_type);
    bb.BitClear(mv.src_tile_index);
    bb.BitSet(mv.dest_tile_index);
}

void PlayerBitboards::DeletePiece(unsigned tile_index) {
    enum PieceType type = GetTile(tile_index);
    Bitboard& bb = GetBitboardByType(type);
    bb.BitClear(tile_index);
}
