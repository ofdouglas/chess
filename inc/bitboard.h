#ifndef BITBOARD_H_DEFINED
#define BITBOARD_H_DEFINED

#include <cstdint>
#include "chess_common.h"


#define A_FILE                  0x0101010101010101ULL
#define H_FILE                  0x8080808080808080ULL
#define RANK_1                  0x00000000000000FFULL
#define RANK_2                  0x000000000000FF00ULL
#define RANK_3                  0x0000000000FF0000ULL
#define RANK_4                  0x00000000FF000000ULL
#define RANK_5                  0x000000FF00000000ULL
#define RANK_6                  0x0000FF0000000000ULL
#define RANK_7                  0x00FF000000000000ULL
#define RANK_8                  0xFF00000000000000ULL
#define LIGHT_SQUARES           0x55AA55AA55AA55AAULL
#define DARK_SQUARES            0xAA55AA55AA55AA55ULL

// TODO: move somewhere else
#define INITIAL_WHITE_PAWNS     0x000000000000FF00ULL
#define INITIAL_WHITE_KNIGHTS   0x0000000000000042ULL
#define INITIAL_WHITE_BISHOPS   0x0000000000000024ULL
#define INITIAL_WHITE_ROOKS     0x0000000000000081ULL
#define INITIAL_WHITE_QUEENS    0x0000000000000008ULL
#define INITIAL_WHITE_KING      0x0000000000000010ULL

// For faster knight attack calculations, shift this knight attack pattern
// (for a knight on D4) so that it's centered on the knight we want to generate for.
#define KNIGHT_PATTERN_D4       0x0000142200221400ULL
#define D4_TILE_INDEX           27

// Represents chess board attributes (such as locations of pieces) via arrays of bits.
// Bit->Tile mapping is in little-endian rank-file format: bit0 -> A1, bit1 -> B1, ..., bit8 -> A2
class Bitboard {
public:
    enum BitscanDirection { BITSCAN_FORWARD, BITSCAN_REVERSE };

    Bitboard();
    Bitboard(uint64_t bits_);

    // Returns the raw bitboard data
    uint64_t GetBits() const;

    // Returns true if the bit at the given index is set
    bool BitTest(unsigned index) const;

    // Sets the bit at the given index
    Bitboard BitSet(unsigned index);

    // Clears the bit at the given index
    Bitboard BitClear(unsigned index);

    // Return a new bitboard consisting of the OR of this bitboard and the other.
    Bitboard LogicOr(Bitboard other) const;

    // Return a new bitboard consisting of the AND of this bitboard and the other.
    Bitboard LogicAnd(Bitboard other) const;

    // Return a new bitboard consisting of the XOR of this bitboard and the other.
    Bitboard LogicXor(Bitboard other) const;

    // Return a new bitboard with all bits flipped 
    Bitboard Invert() const;

    // Finds index of least significant bit that is set. At least one bit must be set.
    unsigned BitscanForward() const ;

    // Finds index of most significant bit that is set. At least one bit must be set.
    unsigned BitscanReverse() const;

    // Parameterized bitscan
    unsigned Bitscan(enum BitscanDirection dir) const;

    // Bitboard single step functions: Shift the whole bitboard by one tile in a cardinal 
    // direction, removing pieces that would shift off the edge. Return new bitboard value.
    Bitboard StepNorth() const;
    Bitboard StepSouth() const;
    Bitboard StepEast() const;
    Bitboard StepWest() const;
    Bitboard StepNorthWest() const;
    Bitboard StepNorthEast() const;
    Bitboard StepSouthWest() const;
    Bitboard StepSouthEast() const;

    // Implements a generic shift by repeated single steps
    Bitboard Shift(int ranks, int files) const;

    bool operator==(const Bitboard& other) const;

    bool operator!=(const Bitboard& other) const;

private:
    uint64_t bits_;
};


/******************************************************************************
 * Bitboard - Inline Function Definitions
 *****************************************************************************/
inline Bitboard::Bitboard() : bits_(0) {}
inline Bitboard::Bitboard(uint64_t x) : bits_(x) {}

inline uint64_t Bitboard::GetBits() const {
    return bits_;
}

inline bool Bitboard::BitTest(unsigned index) const {
    return bits_ & ((uint64_t)1 << index);
}

inline Bitboard Bitboard::BitSet(unsigned index) {
    bits_ |= (uint64_t)1 << index;
    return *this;
}

inline Bitboard Bitboard::BitClear(unsigned index) {
    bits_ &= ~((uint64_t)1 << index);
    return *this;
}

inline Bitboard Bitboard::LogicOr(Bitboard other) const {
    return Bitboard(bits_ | other.GetBits());
}

inline Bitboard Bitboard::LogicAnd(Bitboard other) const {
    return Bitboard(bits_ & other.GetBits());
}

inline Bitboard Bitboard::LogicXor(Bitboard other) const {
    return Bitboard(bits_ ^ other.GetBits());
}

inline Bitboard Bitboard::Invert() const {
    return Bitboard(~bits_);
}

inline unsigned Bitboard::BitscanForward() const {
    assert(bits_);
    return __builtin_ctzll(bits_); // Result of ctz is undefined if bits == 0
}

inline unsigned Bitboard::BitscanReverse() const {
    assert(bits_);
    return 63 - __builtin_clzll(bits_); // Result of clz is undefined if bits == 0
}

// Parameterized bitscan
inline unsigned Bitboard::Bitscan(enum BitscanDirection dir) const {
    return (dir == BITSCAN_FORWARD) ? BitscanForward() : BitscanReverse();
}

// Mask out pieces on the A or H file in these funcs so they shift off the board instead of wrapping
inline Bitboard Bitboard::StepEast() const      { return Bitboard((bits_ & ~H_FILE) << 1); }
inline Bitboard Bitboard::StepWest() const      { return Bitboard((bits_ & ~A_FILE) >> 1); }

inline Bitboard Bitboard::StepNorthWest() const { return Bitboard((bits_ & ~A_FILE) << 7); }
inline Bitboard Bitboard::StepNorthEast() const { return Bitboard((bits_ & ~H_FILE) << 9); }
inline Bitboard Bitboard::StepSouthWest() const { return Bitboard((bits_ & ~A_FILE) >> 9); }
inline Bitboard Bitboard::StepSouthEast() const { return Bitboard((bits_ & ~H_FILE) >> 7); }

// No need for masking - at the edge of the board, these just shift out to zero.
inline Bitboard Bitboard::StepNorth() const     { return Bitboard(bits_ << 8); }
inline Bitboard Bitboard::StepSouth() const     { return Bitboard(bits_ >> 8); }


inline Bitboard Bitboard::Shift(int ranks, int files) const {
    Bitboard b = *this;
    for ( ; files > 0; files--)
        b = b.StepEast();

    for ( ; files < 0; files++)
        b = b.StepWest();

    for ( ; ranks > 0; ranks--)
        b = b.StepNorth();

    for ( ; ranks < 0; ranks++)
        b = b.StepSouth();

    return b;
}

inline bool Bitboard::operator==(const Bitboard& other) const {
    return bits_ == other.bits_;
}

inline bool Bitboard::operator!=(const Bitboard& other) const {
    return !operator==(other);
}

// Represents one player's pieces as a set of bitboards
struct PlayerBitboards {

    // Returns type of piece (or PIECE_TYPE_NONE) which THIS player has at the given index.
    enum PieceType GetTile(unsigned tile_index) const;

    // Return the logical OR (aka union) of all of this player's bitboards.
    Bitboard GetBitboardsUnion() const;

    // Get a reference to one of the bitboards, by piece type
    Bitboard& GetBitboardByType(enum PieceType type);

    // Apply the move to the bitboards
    void MovePiece(Move m);

    // Delete the piece at the given index
    void DeletePiece(unsigned tile_index);

    Bitboard pawns;
    Bitboard knights;
    Bitboard bishops;
    Bitboard rooks;
    Bitboard queens;
    Bitboard king;
};

#endif // BITBOARD_H_DEFINED