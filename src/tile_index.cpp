#include "chess_common.h"
#include <stdexcept>

TileIndex::TileIndex(TileName name)
    : value_(name) {
    if (*this >= num_tiles) {
        throw std::invalid_argument("Attempt to construct TileIndex(" 
            + std::to_string(*this) + ")");
    }
}

TileIndex::TileIndex(unsigned index)
    : value_(static_cast<TileName>(index)) {
    if (*this >= num_tiles) {
        throw std::invalid_argument("Attempt to construct TileIndex(" 
            + std::to_string(*this) + ")");
    }
}

TileIndex::operator unsigned() { 
    return static_cast<unsigned>(value_); 
}

TileIndex::TileIndex(unsigned rank, unsigned file) {
    *this = TileIndex(rank * 8 + file);
}

bool TileIndex::IsValid() {
    return value_ >= TileName::A1 && value_ <= TileName::H8;
}

unsigned TileIndex::Rank() {
    return static_cast<unsigned>(value_) >> 3;
}

unsigned TileIndex::File() {
    return static_cast<unsigned>(value_) & 7;
}

const std::string& TileIndex::Namestring() {
    return name_strings[static_cast<unsigned>(value_)];
}

const std::string TileIndex::name_strings[num_tiles] = {
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
    "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
    "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
};
