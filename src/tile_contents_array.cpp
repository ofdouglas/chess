#include "tile_contents_array.h"

const std::string TileContentsArray::tile_names_[64] = {
    "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8",
    "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8",
    "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8",
    "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8",
    "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
    "G1", "G2", "G3", "G4", "G5", "G6", "G7", "G8",
    "H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8"
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

std::string TileContentsArray::GetTileName(unsigned tile_index) {
    return tile_names_[tile_index];
}

std::string TileContentsArray::GetTileName(unsigned rank, unsigned file) {
    return tile_names_[tile_index_from_rank_file(rank, file)];
}
