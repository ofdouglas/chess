#include "board_state.h"
#include "terminal.h"
#include "chess_engine.h"


enum PlayerType { PLAYER_TYPE_HUMAN, PLAYER_TYPE_CPU };
enum PlayerType player_types[2];   // Indexed by COLOR_WHITE and COLOR_BLACK

int main(int argc, char* argv[]) {
    BoardState board_state = BoardState();
    ChessEngine engine = ChessEngine(); 
    Terminal terminal = Terminal();

    // TODO: a way to quit, other than CTRL-C
    for (;;) {
        terminal.PrintBoard(board_state);

        enum Color to_move = board_state.GetPlayerToMove();
        Move mv;

        if (player_types[to_move] == PLAYER_TYPE_CPU) {
            mv = engine.SelectMove(board_state);
        } else {
            mv = terminal.GetPlayerMove();
        }

        TileContents src_tile = board_state.GetTile(mv.src_tile_index);
        TileContents dest_tile = board_state.GetTile(mv.dest_tile_index);
        
        mv.piece_type = src_tile.piece_type;
        mv.captured_type = dest_tile.piece_type;


        // TODO: consider that PlayerMove may not be legal!
        board_state.ApplyMove(mv);
    }

    return 0;
}
