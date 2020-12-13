#include "bitboard.h"


/******************************************************************************
 * Bitboard Class
 *****************************************************************************/

Bitboard::Bitboard() : bits_(0) {}
Bitboard::Bitboard(uint64_t x) : bits_(x) {}

uint64_t Bitboard::GetBits() const {
    return bits_;
}

bool Bitboard::BitTest(unsigned index) const {
    return bits_ & ((uint64_t)1 << index);
}

void Bitboard::BitSet(unsigned index) {
    bits_ |= (uint64_t)1 << index;
}

void Bitboard::BitClear(unsigned index) {
    bits_ &= ~((uint64_t)1 << index);
}

unsigned Bitboard::BitscanForward() const {
    assert(bits_);
    return __builtin_ctzll(bits_); // Result of ctz is undefined if bits == 0
}

unsigned Bitboard::BitscanReverse() const {
    assert(bits_);
    return 63 - __builtin_clzll(bits_); // Result of clz is undefined if bits == 0
}

// Parameterized bitscan
unsigned Bitboard::Bitscan(enum BitscanDirection dir) const {
    return (dir == BITSCAN_FORWARD) ? BitscanForward() : BitscanReverse();
}

// Mask out pieces on the A or H file in these funcs so they shift off the board instead of wrapping
uint64_t Bitboard::StepEast()      { bits_ = (bits_ & ~H_FILE) >> 1; return bits_; }
uint64_t Bitboard::StepWest()      { bits_ = (bits_ & ~A_FILE) << 1; return bits_; }
uint64_t Bitboard::StepNorthWest() { bits_ = (bits_ & ~A_FILE) << 7; return bits_; }
uint64_t Bitboard::StepNorthEast() { bits_ = (bits_ & ~H_FILE) << 9; return bits_; }
uint64_t Bitboard::StepSouthWest() { bits_ = (bits_ & ~A_FILE) >> 9; return bits_; }
uint64_t Bitboard::StepSouthEast() { bits_ = (bits_ & ~H_FILE) >> 7; return bits_; }

// No need for masking - at the edge of the board, these just shift out to zero.
uint64_t Bitboard::StepNorth()     { bits_ = bits_ << 8; return bits_; }
uint64_t Bitboard::StepSouth()     { bits_ = bits_ >> 8; return bits_; }


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
