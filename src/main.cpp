#include "board_state.h"
#include "terminal.h"
#include "chess_engine.h"

#include <cstdlib>
#include <ctime>

// Just to get names of candidate move tiles
#include "tile_contents_array.h"
void Move::Print() {
    printf("Move %d from %s to %s\n", piece_type, TileContentsArray::GetTileName(src_tile_index), 
        TileContentsArray::GetTileName(dest_tile_index));
}

enum PlayerType { PLAYER_TYPE_CPU, PLAYER_TYPE_HUMAN };
enum PlayerType player_types[2];   // Indexed by COLOR_WHITE and COLOR_BLACK

int main(int argc, char* argv[]) {
    srand(time(NULL));

    BoardState board_state = BoardState();
    ChessEngine engine = ChessEngine(); 
    Terminal terminal = Terminal();

    // TODO: a way to quit, other than CTRL-C
    for (int i = 0; i < 20; i ++) {
        terminal.PrintBoard(board_state);

        enum Color to_move = board_state.GetPlayerToMove();
        Move move;

        if (player_types[to_move] == PLAYER_TYPE_CPU) {
            move = engine.SelectMove(board_state);
            move.Print();

            if (move.captures) {
                TileContents dest_tile = board_state.GetTile(move.dest_tile_index);
                move.captured_type = dest_tile.piece_type;
            }

        } else {
            // NOTE: the player move won't have the captures flag set.
            // Since we're already checking it against a computer generated move that has the flag
            // set, maybe we can just copy it in during the check...
            move = terminal.GetPlayerMove();
            while (!engine.IsLegalMove(board_state, move)) {
                terminal.PrintMessage("Invalid move");
                move = terminal.GetPlayerMove();
            }
            TileContents src_tile = board_state.GetTile(move.src_tile_index);
            move.piece_type = src_tile.piece_type;

            TileContents dest_tile = board_state.GetTile(move.dest_tile_index);
            move.captured_type = dest_tile.piece_type;
            move.captures = move.captured_type != PIECE_TYPE_NONE;
        }

        // TODO: better place to put it?


        // TODO: consider that PlayerMove may not be legal!
        board_state.ApplyMove(move);
    }

    return 0;
}
