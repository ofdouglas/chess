#include "bitboard.h"

/******************************************************************************
 * Player Bitboards Class
 *****************************************************************************/

PieceType PlayerBitboards::GetTile(unsigned tile_index) const {
    if (pawns.BitTest(tile_index))       return PieceType::Pawn;
    if (knights.BitTest(tile_index))     return PieceType::Knight;
    if (bishops.BitTest(tile_index))     return PieceType::Bishop;
    if (rooks.BitTest(tile_index))       return PieceType::Rook;
    if (queens.BitTest(tile_index))      return PieceType::Queen;
    if (king.BitTest(tile_index))        return PieceType::King;

    return PieceType::None;
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
        case PieceType::Pawn:       return pawns;        break;
        case PieceType::Knight:     return knights;      break;
        case PieceType::Bishop:     return bishops;      break;
        case PieceType::Rook:       return rooks;        break;
        case PieceType::Queen:      return queens;       break;
        case PieceType::King:       return king;         break;
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
