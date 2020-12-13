#ifndef TILE_CONTENTS_ARRAY_H_DEFINED
#define TILE_CONTENTS_ARRAY_H_DEFINED

#include "chess_common.h"
#include "board_state.h"

/******************************************************************************
 * Array Of Tiles representation (used for display purposes)
 *****************************************************************************/

class TileContentsArray {
public:
    TileContentsArray();
    TileContentsArray(const BoardState& bs);

    // The tile_index must be in [0,63].
    TileContents GetTile(unsigned tile_index);

    // The rank and file must be in [0,7].
    TileContents GetTile(unsigned rank, unsigned file);

    // Returns tile names "A1" through "H8". The tile_index must be in [0,63].
    static const char* GetTileName(unsigned tile_index);

    // Returns tile names "A1" through "H8". 
    static const char* GetTileName(unsigned rank, unsigned file);

private:
    struct TileContents tiles_[64];
    static const char* tile_names_[64];
};



#endif // TILE_CONTENTS_ARRAY_H_DEFINED