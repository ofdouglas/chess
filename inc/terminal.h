#ifndef TERMINAL_H_DEFINED
#define TERMINAL_H_DEFINED

class Terminal {
public:
    void PrintBoard(const BoardState& bs);
    Move GetPlayerMove();

private:
    void PrintTileFromTileContents(TileContents tc);
    int TileIndexFromText(char file, char rank);
};


#endif