#include "tile_contents_array.h"

const char* TileContentsArray::tile_names_[64] = {
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
    "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
    "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
};


/******************************************************************************
 * Not sure if needed. If so, is there a better place for these helper functions?
 *****************************************************************************/
static unsigned rank_from_tile_index(unsigned tile_index) {
    return tile_index >> 3;
}

static unsigned file_from_tile_index(unsigned tile_index) {
    return tile_index & 7;
}

static unsigned tile_index_from_rank_file(unsigned rank, unsigned file) {
    return rank * 8 + file;
}


TileContents::TileContents(enum Color c, enum PieceType t) : color(c), piece_type(t) {}
TileContents::TileContents() : color(COLOR_NONE), piece_type(PIECE_TYPE_NONE) {}


TileContentsArray::TileContentsArray() {}

TileContentsArray::TileContentsArray(const BoardState& bs) {
    for (int i = 0; i < 64; i++) {
        tiles_[i] = bs.GetTile(i);
    }
}

TileContents TileContentsArray::GetTile(unsigned tile_index) {
    return tiles_[tile_index];
}

TileContents TileContentsArray::GetTile(unsigned rank, unsigned file) {
    return tiles_[tile_index_from_rank_file(rank, file)];
}

const char* TileContentsArray::GetTileName(unsigned tile_index) {
    return tile_names_[tile_index];
}

const char* TileContentsArray::GetTileName(unsigned rank, unsigned file) {
    return tile_names_[tile_index_from_rank_file(rank, file)];
}
