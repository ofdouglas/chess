#ifndef CHESS_COMMON_H_DEFINED
#define CHESS_COMMON_H_DEFINED

// Comment this out to disable assert() macro
#define CHESS_DEBUG_USE_ASSERT

#ifndef CHESS_DEBUG_USE_ASSERT
#define NDEBUG  // disables assert() macro
#endif

#include <cassert>
#include <cstdint>
#include <cstdio>

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

// Index of a tile (aka square) on the chess board
enum TileIndex {
    A1_TILE_INDEX, A2_TILE_INDEX, A3_TILE_INDEX, A4_TILE_INDEX,
    A5_TILE_INDEX, A6_TILE_INDEX, A7_TILE_INDEX, A8_TILE_INDEX,
    B1_TILE_INDEX, B2_TILE_INDEX, B3_TILE_INDEX, B4_TILE_INDEX,
    B5_TILE_INDEX, B6_TILE_INDEX, B7_TILE_INDEX, B8_TILE_INDEX,
    C1_TILE_INDEX, C2_TILE_INDEX, C3_TILE_INDEX, C4_TILE_INDEX,
    C5_TILE_INDEX, C6_TILE_INDEX, C7_TILE_INDEX, C8_TILE_INDEX,
    D1_TILE_INDEX, D2_TILE_INDEX, D3_TILE_INDEX, D4_TILE_INDEX,
    D5_TILE_INDEX, D6_TILE_INDEX, D7_TILE_INDEX, D8_TILE_INDEX,
    E1_TILE_INDEX, E2_TILE_INDEX, E3_TILE_INDEX, E4_TILE_INDEX,
    E5_TILE_INDEX, E6_TILE_INDEX, E7_TILE_INDEX, E8_TILE_INDEX,
    F1_TILE_INDEX, F2_TILE_INDEX, F3_TILE_INDEX, F4_TILE_INDEX,
    F5_TILE_INDEX, F6_TILE_INDEX, F7_TILE_INDEX, F8_TILE_INDEX,
    G1_TILE_INDEX, G2_TILE_INDEX, G3_TILE_INDEX, G4_TILE_INDEX,
    G5_TILE_INDEX, G6_TILE_INDEX, G7_TILE_INDEX, G8_TILE_INDEX,
    H1_TILE_INDEX, H2_TILE_INDEX, H3_TILE_INDEX, H4_TILE_INDEX,
    H5_TILE_INDEX, H6_TILE_INDEX, H7_TILE_INDEX, H8_TILE_INDEX
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

// TODO: could move some of these fields out to an ExtendedMove structure
// since it seems they will only be needed for an UndoMove function
struct Move {
    Move() : 
        src_tile_index(0),
        dest_tile_index(0), 
        piece_type(PIECE_TYPE_NONE), 
        captured_type(PIECE_TYPE_NONE), 
        promotion_type(PIECE_TYPE_NONE),
        captures(0) {}

    uint8_t src_tile_index;
    uint8_t dest_tile_index;
    enum PieceType piece_type;
    enum PieceType captured_type;
    enum PieceType promotion_type;
    CastlingRights prev_castling_rights;

    // During move generation we do not look up the type of piece that would be captured,
    // since that would waste a lot of time. Instead we just set this flag, and the 'captured_type'
    // gets set later (when the move is applied)
    bool captures;
};

#define BITS_PER_RANK 8
#define BITS_PER_FILE 1

#define NORTH_OFFSET         8
#define SOUTH_OFFSET        -8
#define EAST_OFFSET          1
#define WEST_OFFSET         -1
#define NORTHWEST_OFFSET     7
#define NORTHEAST_OFFSET     9
#define SOUTHWEST_OFFSET    -9
#define SOUTHEAST_OFFSET    -7


inline unsigned RankFromTileIndex(unsigned tile_index) {
    return tile_index >> 3;
}

inline unsigned FileFromTileIndex(unsigned tile_index) {
    return tile_index & 7;
}

inline unsigned TileIndexFromRankAndFile(unsigned rank, unsigned file) {
    return rank * 8 + file;
}



#endif // CHESS_COMMON_H_DEFINED