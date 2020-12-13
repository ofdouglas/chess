#ifndef CHESS_ENGINE_H_DEFINED
#define CHESS_ENGINE_H_DEFINED

#include "chess_common.h"

class ChessEngine {
public:
    ChessEngine() {}
    Move SelectMove(const BoardState& bs) { return Move(); }
};


#endif // CHESS_ENGINE_H_DEFINED