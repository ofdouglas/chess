#include "bitboard.h"
#include <sstream>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const Bitboard& b) {
    std::stringstream sstream;

    // TODO: would be easier to read if printed as: 00 08 08 08 00 00 00 00
    sstream << "Bitboard\n0x" << std::hex << std::setw(16) << std::setfill('0') << b.GetBits() << "\n";

    for (int rank = 7; rank >= 0; rank--) {
        sstream << static_cast<char>('A' + rank) << " ";
        for (int file = 0; file <= 7; file++) {
            sstream << (b.BitTest(TileIndex(rank, file)) ? " X " : " . ");
        }
        sstream << "\n";
    }

    sstream << "   1  2  3  4  5  6  7  8\n";
    os << sstream.str();
    return os;
}

/******************************************************************************
 * Player Bitboards Class
 *****************************************************************************/

PieceType PlayerBitboards::GetTile(TileIndex index) const {
    if (pawns.BitTest(index))       return PieceType::Pawn;
    if (knights.BitTest(index))     return PieceType::Knight;
    if (bishops.BitTest(index))     return PieceType::Bishop;
    if (rooks.BitTest(index))       return PieceType::Rook;
    if (queens.BitTest(index))      return PieceType::Queen;
    if (king.BitTest(index))        return PieceType::King;

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

void PlayerBitboards::DeletePiece(TileIndex index) {
    enum PieceType type = GetTile(index);
    Bitboard& bb = GetBitboardByType(type);
    bb.BitClear(index);
}
