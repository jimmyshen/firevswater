/**
 * Fire vs Water : Core game library
 *
 * This source file implements the functionality behind the
 * game library.
 *
 * Copyright (C) 2011 Chun Min Shen <kuato@erratical.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include "fvw_debug.h"
#include "fvw_core.h"

#define FVW_GET_PIECE(board, x, y)  board->pieces[(y * board->dim + x)]

void _fvw_MarkMovesAux(
        fvw_GameBoard* board,
        fvw_GameBoard* markers,
        int x,
        int y,
        int prev_x,
        int prev_y,
        int d );

inline FVW_BOOL fvw_IsWithinBounds(fvw_GameBoard* board, int x, int y) {
    ASSERT(board != NULL);
    return (0 <= x && x < board->dim &&
               0 <= y && y < board->dim);
}

inline FVW_BOOL fvw_IsOutsideBounds(fvw_GameBoard* board, int x, int y) {
    ASSERT(board != NULL);
    return (0 > x || x >= board->dim ||
               0 > y || y >= board->dim);
}

fvw_GameBoard* fvw_CreateBoard(size_t dimension) {
    TRACE_FN();

    size_t board_size = sizeof(fvw_GameBoard);
    fvw_GameBoard* game_board = malloc(board_size);
    if ( game_board != NULL ) {
        VTRACE("allocated %lu bytes for game board of size at %p", board_size, game_board);
        memset(game_board, 0, board_size);

        size_t area = dimension * dimension;
        size_t pieces_size = sizeof(fvw_Piece) * area;
        if ( (game_board->pieces = malloc(pieces_size)) != NULL ) {
            VTRACE("allocated %lu bytes to store %lu pieces (%lux%lu)",
                    pieces_size, area, dimension, dimension);

            game_board->dim = dimension;
            game_board->size = area;
            memset(game_board->pieces, FVW_PIECE_NONE, pieces_size);
        } else {
            VTRACE("failed to allocate %lu bytes to store %lu pieces (%lux%lu)",
                    pieces_size, area, dimension, dimension);
            free(game_board);
            game_board = NULL;
        }
    }

    return game_board;
}

void fvw_DestroyBoard(fvw_GameBoard** board) {
    TRACE_FN();
    if ( board != NULL && *board != NULL ) {
        if ( (*board)->pieces != NULL ) {
            free((*board)->pieces);
            (*board)->pieces = NULL;
        }
        VTRACE("freed game board at %p", *board);
        free(*board);
        *board = NULL;
    }
}

inline void fvw_ClearBoard(fvw_GameBoard* board) {
    TRACE_FN();
    ASSERT(board != NULL && board->pieces != NULL);

    memset(board->pieces, FVW_PIECE_NONE, sizeof(fvw_Piece) * board->size);
}

void fvw_ResetPieces(fvw_GameBoard* board) {
    TRACE_FN();
    ASSERT(board != NULL && board->pieces != NULL);

    fvw_ClearBoard(board);

    int x, y, len = (board->dim >> 1);
    for ( y = 0; y < len; ++y ) {
        for ( x = 0; x < (len - y); ++x ) {
            FVW_GET_PIECE(board, x, y) = FVW_PIECE_WATER;
            FVW_GET_PIECE(board,
                    (board->dim - x - 1),
                    (board->dim - y - 1)) = FVW_PIECE_FIRE;
        }
    }
}

FVW_BOOL fvw_GetPiece(fvw_GameBoard* board, int x, int y, fvw_Piece* piece) {
    TRACE_FN();
    ASSERT(board != NULL && piece != NULL);

    if ( fvw_IsOutsideBounds(board, x, y) ) {
        VTRACE("outside of bounds at %d, %d", x, y);
        return FVW_FALSE;
    }

    *piece = FVW_GET_PIECE(board, x, y);

    return FVW_TRUE;
}

FVW_BOOL fvw_SetPiece(fvw_GameBoard* board, int x, int y, fvw_Piece piece) {
    TRACE_FN();
    ASSERT(board != NULL);

    FVW_BOOL ret = FVW_FALSE;

    if ( fvw_IsWithinBounds(board, x, y) ) {
        FVW_GET_PIECE(board, x, y) = piece;
        ret = FVW_TRUE;
    }

    return ret;
}

FVW_BOOL fvw_MovePiece(
    fvw_GameBoard* board,
    int       src_x,
    int       src_y,
    int       dst_x,
    int       dst_y) {
    TRACE_FN();
    ASSERT(board != NULL);

    FVW_BOOL ret = FVW_FALSE;

    if ( fvw_IsWithinBounds(board, src_x, src_y) &&
         fvw_IsWithinBounds(board, dst_x, dst_y) &&
         FVW_GET_PIECE(board, src_x, src_y) != FVW_PIECE_NONE ) {

        FVW_GET_PIECE(board, dst_x, dst_y) =
            FVW_GET_PIECE(board, src_x, src_y);

        FVW_GET_PIECE(board, src_x, src_y) = FVW_PIECE_NONE;

        ret = FVW_TRUE;
    }

    return ret;
}

FVW_BOOL fvw_MarkMoves(fvw_GameBoard* board, int x, int y, fvw_GameBoard* result) {
    TRACE_FN();
    ASSERT(board != NULL && result != NULL);

    FVW_BOOL ret = FVW_FALSE;

    if ( fvw_IsWithinBounds(board, x, y) ) {
        fvw_ClearBoard(result);

        FVW_GET_PIECE(result, x, y) = FVW_PIECE_ORIGIN;

        if ( x > 0 &&
                FVW_GET_PIECE(board, (x - 1), y) == FVW_PIECE_NONE )
            FVW_GET_PIECE(result, (x - 1), y) = FVW_PIECE_MARKER;

        if ( y > 0 &&
                FVW_GET_PIECE(board, x, (y - 1)) == FVW_PIECE_NONE )
            FVW_GET_PIECE(result, x, (y - 1)) = FVW_PIECE_MARKER;

        if ( x < (board->dim - 1) &&
                FVW_GET_PIECE(board, (x + 1), y) == FVW_PIECE_NONE )
            FVW_GET_PIECE(result, (x + 1), y) = FVW_PIECE_MARKER;

        if ( y < (board->dim - 1) &&
                FVW_GET_PIECE(board, x, (y + 1)) == FVW_PIECE_NONE )
            FVW_GET_PIECE(result, x, (y + 1)) = FVW_PIECE_MARKER;

        _fvw_MarkMovesAux(board, result, x - 1, y, x, y, 1);
        _fvw_MarkMovesAux(board, result, x, y - 1, x, y, 1);
        _fvw_MarkMovesAux(board, result, x + 1, y, x, y, 1);
        _fvw_MarkMovesAux(board, result, x, y + 1, x, y, 1);

        ret = FVW_TRUE;
    }

    return ret;
}

void _fvw_MarkMovesAux(
        fvw_GameBoard* board,
        fvw_GameBoard* markers,
        int x,
        int y,
        int prev_x,
        int prev_y,
        int d) {

    if ( fvw_IsOutsideBounds(board, x, y) )
        return;

    // do not process if already visited
    if ( FVW_GET_PIECE(markers, x, y) != FVW_PIECE_NONE )
        return;

    // mark this spot as visited
    FVW_GET_PIECE(markers, x, y) = FVW_PIECE_VISITED;

    // allow moves that 'jump' over adjacent pieces...
    if ( d >= 1 ) {
        if ( (d % 2) == 0 &&
                FVW_GET_PIECE(board, x, y) == FVW_PIECE_NONE ) {
            FVW_GET_PIECE(markers, x, y) = FVW_PIECE_MARKER;
            ++d;
            _fvw_MarkMovesAux(board, markers, x - 1, y, x, y, d);
            _fvw_MarkMovesAux(board, markers, x + 1, y, x, y, d);
            _fvw_MarkMovesAux(board, markers, x, y - 1, x, y, d);
            _fvw_MarkMovesAux(board, markers, x, y + 1, x, y, d);
        } else if ( (d % 2 - 1) == 0 &&
                        FVW_GET_PIECE(board, x, y) != FVW_PIECE_NONE) {
            FVW_GET_PIECE(markers, x, y) = FVW_PIECE_VISITED;
            ++d;
            if ( x < prev_x )
                _fvw_MarkMovesAux(board, markers, x - 1, y, x, y, d);
            if ( x > prev_x )
                _fvw_MarkMovesAux(board, markers, x + 1, y, x, y, d);
            if ( y < prev_y )
                _fvw_MarkMovesAux(board, markers, x, y - 1, x, y, d);
            if ( y > prev_y )
                _fvw_MarkMovesAux(board, markers, x, y + 1, x, y, d);
        }
    }
}

