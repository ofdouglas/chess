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

enum class TileName : uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};

// Index of a tile (aka square) on the chess board
// TODO: probably should go in it's own header.
class TileIndex {
public:
    static constexpr unsigned num_tiles = 64;

    TileName value_;

    operator unsigned();
    
    // TODO: TileContents array version of this is now redundant
    static const std::string name_strings[num_tiles];
    const std::string& Namestring();

    TileIndex(TileName name);
    TileIndex(unsigned index);
    TileIndex(unsigned rank, unsigned file);

    unsigned Rank();
    unsigned File();
    bool IsValid();
};


struct TileContents {
    TileContents(Color c, PieceType t) : color(c), piece_type(t) {}
    TileContents() : color(Color::None), piece_type(PieceType::None) {}

    Color color;
    PieceType piece_type;
};

struct CastlingRights {
    CastlingRights() 
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