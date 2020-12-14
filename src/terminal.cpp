#include "chess_common.h"
#include "tile_contents_array.h"
#include "terminal.h"

#include <iostream>
#include <string>


void Terminal::PrintTileFromTileContents(TileContents tc) {
    const char* piece = 0;

    if (tc.color == COLOR_BLACK) {
        switch (tc.piece_type) {
            case PIECE_TYPE_KING:       piece = "♔";  break;
            case PIECE_TYPE_QUEEN:      piece = "♕";  break;
            case PIECE_TYPE_ROOK:       piece = "♖";  break;
            case PIECE_TYPE_BISHOP:     piece = "♗";  break;
            case PIECE_TYPE_KNIGHT:     piece = "♘";  break;
            case PIECE_TYPE_PAWN:       piece = "♙";  break;
            default:                    piece = "▨";  break;
        }
    } else {
        switch (tc.piece_type) {
            case PIECE_TYPE_KING:       piece = "♚";  break;
            case PIECE_TYPE_QUEEN:      piece = "♛";  break;
            case PIECE_TYPE_ROOK:       piece = "♜";  break;
            case PIECE_TYPE_BISHOP:     piece = "♝";  break;
            case PIECE_TYPE_KNIGHT:     piece = "♞";  break;
            case PIECE_TYPE_PAWN:       piece = "♟";  break;
            default:                    piece = "□";  break;
        }
    }
    printf("%s", piece);
}

void Terminal::PrintMessage(const char* msg) {
    puts(msg);
}

void Terminal::PrintBoard(const BoardState& bs) {
    TileContentsArray board(bs);

    puts("");
    printf("  ");
    for (int file = 0; file < 8; file++) {
        printf("%c ", 'a' + file);
    }
    puts("");
    
    for (int rank = 7; rank >= 0; rank--) {
        printf("%d ", rank + 1);

        for (int file = 0; file <= 7; file++) {
            unsigned tile_index = rank * 8 + file;

            // Set color of empty tiles to get board background
            TileContents tile = board.GetTile(tile_index);
            if (tile.piece_type == PIECE_TYPE_NONE) {
                tile.color = ((rank + file) & 1) ? COLOR_WHITE : COLOR_BLACK;
            }
            PrintTileFromTileContents(tile);
            printf(" ");
        }
        printf(" %d ", rank + 1);
        puts("");
    }

    printf("  ");
    for (int file = 0; file < 8; file++) {
        printf("%c ", 'a' + file);
    }
    printf("\n%s to move: \n\n", bs.GetPlayerToMove() == COLOR_WHITE ? "WHITE" : "BLACK");
}

// Returns -1 if invalid tile index
int Terminal::TileIndexFromText(char file, char rank) {
    if ((rank >= '1' && rank <= '8') && 
        (file >= 'a' && file <= 'h')) {
        return (rank - '1') * 8 + (file - 'a');
    }

    return -1;
}

Move Terminal::GetPlayerMove() {
    Move mv;
    std::string input;

    while (true) {
        std::getline(std::cin, input);

        if (input.size() == 4) {
            int from = TileIndexFromText(input[0], input[1]);
            int to = TileIndexFromText(input[2], input[3]);

            if (to > 0 && from > 0) {
                mv.src_tile_index = from;
                mv.dest_tile_index = to;
                std::cout << "Parsed move from/to indexes " << from << "/" << to << std::endl; 
                break;
            }
        }

        std::cout << "Error parsing move" << std::endl;
    }

    return mv;
}