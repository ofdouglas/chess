#ifndef BITBOARD_H_DEFINED
#define BITBOARD_H_DEFINED

#include <cstdint>
#include "chess_common.h"

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
    void BitSet(unsigned index);

    // Clears the bit at the given index
    void BitClear(unsigned index);

    // Finds index of least significant bit that is set. At least one bit must be set.
    unsigned BitscanForward() const ;

    // Finds index of most significant bit that is set. At least one bit must be set.
    unsigned BitscanReverse() const;

    // Parameterized bitscan
    unsigned Bitscan(enum BitscanDirection dir) const;

    // Bitboard single step functions: Shift the whole bitboard by one tile in a cardinal 
    // direction, removing pieces that would shift off the edge. Return new bitboard value.
    uint64_t StepNorth();
    uint64_t StepSouth();
    uint64_t StepEast();
    uint64_t StepWest();
    uint64_t StepNorthWest();
    uint64_t StepNorthEast();
    uint64_t StepSouthWest();
    uint64_t StepSouthEast();

private:
    uint64_t bits_;
};


// Represents one player's pieces as a set of bitboards
struct PlayerBitboards {

    // Returns type of piece (or PIECE_TYPE_NONE) which THIS player has at the given index.
    enum PieceType GetTile(unsigned tile_index) const;

    // Return the logical OR (aka union) of all of this player's bitboards.
    Bitboard GetBitboardsUnion() const;

    Bitboard pawns;
    Bitboard knights;
    Bitboard bishops;
    Bitboard rooks;
    Bitboard queens;
    Bitboard king;
};


#define A_FILE                  0x0101010101010101ULL
#define H_FILE                  0x8080808080808080ULL
#define RANK_1                  0x00000000000000FFULL
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


#endif // BITBOARD_H_DEFINED