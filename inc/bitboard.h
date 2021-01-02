#ifndef BITBOARD_H_DEFINED
#define BITBOARD_H_DEFINED

#include <cstdint>
#include "chess_common.h"


// Represents chess board attributes (such as locations of pieces) via arrays of bits.
// Bit->Tile mapping is in little-endian rank-file format: bit0 -> A1, bit1 -> B1, ..., bit8 -> A2
class Bitboard {
public:
    enum class BitscanDirection { Forward, Reverse };

    // TODO: It's potentially cleaner to make these const Bitboards, in a BitboardConstants namespace.
    static constexpr uint64_t rank_1_bits = 0x00000000000000FFULL;
    static constexpr uint64_t rank_2_bits = 0x000000000000FF00ULL;
    static constexpr uint64_t rank_3_bits = 0x0000000000FF0000ULL;
    static constexpr uint64_t rank_4_bits = 0x00000000FF000000ULL;
    static constexpr uint64_t rank_5_bits = 0x000000FF00000000ULL;
    static constexpr uint64_t rank_6_bits = 0x0000FF0000000000ULL;
    static constexpr uint64_t rank_7_bits = 0x00FF000000000000ULL;
    static constexpr uint64_t rank_8_bits = 0xFF00000000000000ULL;
    static constexpr uint64_t a_file_bits = 0x0101010101010101ULL;
    static constexpr uint64_t h_file_bits = 0x8080808080808080ULL;
    static constexpr uint64_t light_square_bits = 0x55AA55AA55AA55AAULL;
    static constexpr uint64_t dark_square_bits = 0xAA55AA55AA55AA55ULL;

    static constexpr uint64_t initial_white_pawn_bits = 0x000000000000FF00ULL;
    static constexpr uint64_t initial_white_knight_bits = 0x0000000000000042ULL;
    static constexpr uint64_t initial_white_bishop_bits = 0x0000000000000024ULL;
    static constexpr uint64_t initial_white_rook_bits = 0x0000000000000081ULL;
    static constexpr uint64_t initial_white_queen_bits = 0x0000000000000008ULL;
    static constexpr uint64_t initial_white_king_bits = 0x0000000000000010ULL;

    // For faster knight attack calculations, shift this knight attack pattern
    // (for a knight on D4) so that it's centered on the knight we want to generate for.
    static constexpr uint64_t knight_pattern_d4 = 0x0000142200221400ULL;

    Bitboard(uint64_t bits_ = 0);
    Bitboard(TileIndex index);

    // Returns the raw bitboard data
    uint64_t GetBits() const;

    // Returns true if the bit at the given index is set
    bool BitTest(unsigned index) const;

    // Sets the bit at the given index
    Bitboard& BitSet(unsigned index);

    // Clears the bit at the given index
    Bitboard& BitClear(unsigned index);

    // Bitwise operators
    Bitboard operator|(const Bitboard& other) const;
    Bitboard& operator|=(const Bitboard& other);

    Bitboard operator&(const Bitboard& other) const;
    Bitboard& operator&=(const Bitboard& other);

    Bitboard operator^(const Bitboard& other) const;
    Bitboard& operator^=(const Bitboard& other);

    Bitboard operator~() const;

    // Finds index of least significant bit that is set. At least one bit must be set.
    unsigned BitscanForward() const ;

    // Finds index of most significant bit that is set. At least one bit must be set.
    unsigned BitscanReverse() const;

    // Parameterized bitscan
    unsigned Bitscan(BitscanDirection dir) const;

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

    // Equals and not equals operators
    bool operator==(const Bitboard& other) const;
    bool operator!=(const Bitboard& other) const;

private:
    uint64_t bits_;
};


/******************************************************************************
 * Bitboard - Inline Function Definitions
 *****************************************************************************/
inline Bitboard::Bitboard(uint64_t x) : bits_(x) {}

inline Bitboard::Bitboard(TileIndex index)
    : bits_((uint64_t)1 << static_cast<int>(index.value_)) {}

inline uint64_t Bitboard::GetBits() const {
    return bits_;
}

inline bool Bitboard::BitTest(unsigned index) const {
    return bits_ & ((uint64_t)1 << index);
}

inline Bitboard& Bitboard::BitSet(unsigned index) {
    bits_ |= (uint64_t)1 << index;
    return *this;
}

inline Bitboard& Bitboard::BitClear(unsigned index) {
    bits_ &= ~((uint64_t)1 << index);
    return *this;
}

inline Bitboard Bitboard::operator|(const Bitboard& other) const {
    return Bitboard(bits_ | other.GetBits());
}

inline Bitboard& Bitboard::operator|=(const Bitboard& other) {
    bits_ |= other.bits_;
    return *this;
}

inline Bitboard Bitboard::operator&(const Bitboard& other) const {
    return Bitboard(bits_ & other.GetBits());
}

inline Bitboard& Bitboard::operator&=(const Bitboard& other) {
    bits_ &= other.bits_;
    return *this;
}

inline Bitboard Bitboard::operator^(const Bitboard& other) const {
    return Bitboard(bits_ ^ other.GetBits());
}

inline Bitboard& Bitboard::operator^=(const Bitboard& other) {
    bits_ ^= other.bits_;
    return *this;
}

inline Bitboard Bitboard::operator~() const {
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
inline unsigned Bitboard::Bitscan(BitscanDirection dir) const {
    return (dir == BitscanDirection::Forward) ? BitscanForward() : BitscanReverse();
}

// Mask out pieces on the A or H file in these funcs so they shift off the board instead of wrapping
inline Bitboard Bitboard::StepEast() const      { return Bitboard((bits_ & ~h_file_bits) << 1); }
inline Bitboard Bitboard::StepWest() const      { return Bitboard((bits_ & ~a_file_bits) >> 1); }

inline Bitboard Bitboard::StepNorthWest() const { return Bitboard((bits_ & ~a_file_bits) << 7); }
inline Bitboard Bitboard::StepNorthEast() const { return Bitboard((bits_ & ~h_file_bits) << 9); }
inline Bitboard Bitboard::StepSouthWest() const { return Bitboard((bits_ & ~a_file_bits) >> 9); }
inline Bitboard Bitboard::StepSouthEast() const { return Bitboard((bits_ & ~h_file_bits) >> 7); }

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

    // Returns type of piece (or PieceType::None) which THIS player has at the given index.
    PieceType GetTile(TileIndex index) const;

    // Return the logical OR (aka union) of all of this player's bitboards.
    Bitboard GetBitboardsUnion() const;

    // Get a reference to one of the bitboards, by piece type
    Bitboard& GetBitboardByType(PieceType type);

    // Apply the move to the bitboards
    void MovePiece(Move m);

    // Delete the piece at the given index
    void DeletePiece(TileIndex index);

    Bitboard pawns;
    Bitboard knights;
    Bitboard bishops;
    Bitboard rooks;
    Bitboard queens;
    Bitboard king;
};

#endif // BITBOARD_H_DEFINED