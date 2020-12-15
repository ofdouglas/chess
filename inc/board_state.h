#ifndef BOARD_STATE_H_DEFINED
#define BOARD_STATE_H_DEFINED

#include <string>

#include "chess_common.h"
#include "bitboard.h"

// The state of the chess board (also known as a 'position')
class BoardState {
public:

    // Set up the board for the start of a standard game.
    BoardState();

    // Initialize the board to the given state, which must be in FEN notation.
    // TODO: not implemented yet! But the empty string can be passed to get an empty board.
    BoardState(std::string init_state_fen);

    enum Color GetPlayerToMove() const;

    PlayerBitboards& GetSelfBitboards();

    PlayerBitboards& GetOpponentBitboards();

    // Useful for converting from bitboard representation to array representation of the board.
    TileContents GetTile(unsigned tile_index) const;

    // Useful for test purposes (e.g. adding pieces). Probably doesn't have any use in a game.
    void SetTile(unsigned tile_index, TileContents tc);

    // Update board state according to m and return true, if m is valid. Else return false.
    bool ApplyMove(Move m);



private:
    struct PlayerBitboards bitboards[2];    // Player pieces, indexed by COLOR_WHITE or COLOR_BLACK
    struct CastlingRights castling[2];      // Player castling info, indexed by COLOR_WHITE or COLOR_BLACK
    Bitboard en_passant_target_bitboard;    // Tiles where en passant capture is legal, in this ply
    unsigned ply_counter;                   // Zero indexed (white moves on ply 0, 2, 4...)

    // TODO: implement 50 move rule, which requires the half_move_counter
    //unsigned half_move_counter;             // Num half turns since the last capture / pawn move. Draw at 100.
};

#endif // BOARD_STATE_H_DEFINED