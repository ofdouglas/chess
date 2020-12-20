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
#include <string>

/******************************************************************************
 * Common definitions
 *****************************************************************************/
// Color::Black and Color::White need to remain as 0 and 1 (or 1 and 0)
// because they are converted to int and used as array indexes.
enum class Color {
    Black = 0,
    White = 1,
    None
};

enum class Direction {
    North,
    South,
    East,
    West,
    NorthEast,
    SouthEast,
    SouthWest,
    NorthWest
};

enum class PieceType {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None
};

// Index of a tile (aka square) on the chess board
// TODO: probably should go in it's own header.
class TileIndex {
public:
    static constexpr unsigned num_tiles = 64;

    enum class TileName : uint8_t {
        A1, A2, A3, A4, A5, A6, A7, A8,
        B1, B2, B3, B4, B5, B6, B7, B8,
        C1, C2, C3, C4, C5, C6, C7, C8,
        D1, D2, D3, D4, D5, D6, D7, D8,
        E1, E2, E3, E4, E5, E6, E7, E8,
        F1, F2, F3, F4, F5, F6, F7, F8,
        G1, G2, G3, G4, G5, G6, G7, G8,
        H1, H2, H3, H4, H5, H6, H7, H8
    } value_;

    // TODO: TileContents array version of this is now redundant
    static const std::string name_strings[num_tiles];

    TileIndex(TileName name);
    TileIndex(int index);
    TileIndex(int rank, int file);

    int ToInt();
    bool Isvalid();

    int Rank();
    int File();
};


struct TileContents {
    TileContents(Color c, PieceType t) : color(c), piece_type(t) {}
    TileContents() : color(Color::None), piece_type(PieceType::None) {}

    Color color;
    PieceType piece_type;
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
        piece_type(PieceType::None), 
        captured_type(PieceType::None), 
        promotion_type(PieceType::None),
        captures(0) {}

    uint8_t src_tile_index;
    uint8_t dest_tile_index;
    PieceType piece_type;
    PieceType captured_type;
    PieceType promotion_type;
    CastlingRights prev_castling_rights;

    // During move generation we do not look up the type of piece that would be captured,
    // since that would waste a lot of time. Instead we just set this flag, and the 'captured_type'
    // gets set later (when the move is applied)
    bool captures;
};

// Returns the change in tile index associated with a single step in the given direction.
inline int TileIndexOffsetFromDirection(Direction dir) {
    switch(dir) {
        case Direction::North:          return  8;  break;
        case Direction::South:          return -8;  break;
        case Direction::East:           return  1;  break;
        case Direction::West:           return -1;  break;
        case Direction::NorthEast:      return  9;  break;
        case Direction::NorthWest:      return  7;  break;
        case Direction::SouthEast:      return -7;  break;
        case Direction::SouthWest:      return -9;  break;
        default:                        return  0;  break;
    }
}

#endif // CHESS_COMMON_H_DEFINED