#ifndef CHESS_COMMON_H_DEFINED
#define CHESS_COMMON_H_DEFINED

// Comment this out to disable assert() macro
#define CHESS_DEBUG_USE_ASSERT

#ifndef CHESS_DEBUG_USE_ASSERT
#define NDEBUG  // disables assert() macro
#endif

#include <cassert>
#include <cstdint>

/******************************************************************************
 * Common definitions
 *****************************************************************************/
enum Color {
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_NONE
};

enum Direction {
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_WEST,
    DIRECTION_NORTHEAST,
    DIRECTION_SOUTHEAST,
    DIRECTION_SOUTHWEST,
    DIRECTION_NORTHWEST
};

enum PieceType {
    PIECE_TYPE_PAWN,
    PIECE_TYPE_KNIGHT,
    PIECE_TYPE_BISHOP,
    PIECE_TYPE_ROOK,
    PIECE_TYPE_KING,
    PIECE_TYPE_QUEEN,
    PIECE_TYPE_NONE,
    PIECE_TYPE_NUM_ELEMENTS
};

struct TileContents {
    TileContents(enum Color c, enum PieceType t);
    TileContents();

    enum Color color;
    enum PieceType piece_type;
};

struct CastlingRights {
    inline CastlingRights() 
        : rook_a_has_moved(false), rook_h_has_moved(false), king_has_moved(false) {}

    unsigned rook_a_has_moved : 1;
    unsigned rook_h_has_moved : 1;
    unsigned king_has_moved   : 1;
};

struct Move {
    uint8_t src_tile_index;
    uint8_t dest_tile_index;
    enum PieceType piece_type;
    enum PieceType captured_type;
    enum PieceType promotion_type;
    CastlingRights prev_castling_rights;
};

#define BITS_PER_RANK 8
#define BITS_PER_FILE 1


#endif // CHESS_COMMON_H_DEFINED