#ifndef TERMINAL_H_DEFINED
#define TERMINAL_H_DEFINED

#include "board_state.h"

class Terminal {
public:
    void PrintBoard(const BoardState& bs);
    Move GetPlayerMove();
    void PrintMessage(const char* msg);

private:
    void PrintTileFromTileContents(TileContents tc);
    int TileIndexFromText(char file, char rank);
};


#endif