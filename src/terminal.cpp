#include "chess_common.h"
#include "terminal.h"

#include <iostream>
#include <string>


void Terminal::PrintTileFromTileContents(TileContents tc) {
    const char* piece = 0;

    if (tc.color == Color::Black) {
        switch (tc.piece_type) {
            case PieceType::King:       piece = "♔";  break;
            case PieceType::Queen:      piece = "♕";  break;
            case PieceType::Rook:       piece = "♖";  break;
            case PieceType::Bishop:     piece = "♗";  break;
            case PieceType::Knight:     piece = "♘";  break;
            case PieceType::Pawn:       piece = "♙";  break;
            default:                    piece = "▨";  break;
        }
    } else {
        switch (tc.piece_type) {
            case PieceType::King:       piece = "♚";  break;
            case PieceType::Queen:      piece = "♛";  break;
            case PieceType::Rook:       piece = "♜";  break;
            case PieceType::Bishop:     piece = "♝";  break;
            case PieceType::Knight:     piece = "♞";  break;
            case PieceType::Pawn:       piece = "♟";  break;
            default:                    piece = "□";  break;
        }
    }
    printf("%s", piece);
}

void Terminal::PrintMessage(const char* msg) {
    puts(msg);
}

void Terminal::PrintBoard(const BoardState& bs) {
    TileContents board[TileIndex::num_tiles];
    for (int i = 0; i < TileIndex::num_tiles; i++) {
        board[i] = bs.GetTile(i);
    }

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
            TileContents tile = board[tile_index];
            if (tile.piece_type == PieceType::None) {
                tile.color = ((rank + file) & 1) ? Color::White : Color::Black;
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
    double evaluation = bs.GetEvaluation();
    printf("\nEvaluation: %s%.1f\n", evaluation > 0 ? "+" : "", evaluation);
    printf("%s to move: \n\n", bs.GetPlayerToMove() == Color::White ? "WHITE" : "BLACK");
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