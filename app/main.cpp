#include "board_state.h"
#include "terminal.h"
#include "chess_engine.h"

#include <cstdlib>
#include <ctime>


// void Move::Print() {
//     printf("Move %d from %s to %s\n", piece_type, TileContentsArray::GetTileName(src_tile_index), 
//         TileContentsArray::GetTileName(dest_tile_index));
// }

enum PlayerType { Cpu, Human } player_types[2];   // Indexed by COLOR_WHITE and COLOR_BLACK

int main(int argc, char* argv[]) {
    srand(time(NULL));

    BoardState board_state = BoardState();
    ChessEngine engine = ChessEngine(); 
    Terminal terminal = Terminal();

    // TODO: a way to quit, other than CTRL-C
    for (int i = 0; i < 20; i ++) {
        terminal.PrintBoard(board_state);

        int to_move = static_cast<int>(board_state.GetPlayerToMove());
        Move move;

        if (player_types[to_move] == PlayerType::Cpu) {
            move = engine.SelectMove(board_state);
            //move.Print();

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
            move.captures = move.captured_type != PieceType::None;
        }
        
        board_state.ApplyMove(move);
    }

    return 0;
}
