/**
 * Fire vs Water : Core game library
 *
 * Interface for creating, releasing and using data structures used
 * for the game.

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

#ifndef FVW_CORE_H
#define FVW_CORE_H

typedef enum {
  FVW_FALSE = 0,
  FVW_TRUE  = 1
} FVW_BOOL;

typedef enum {
  FVW_PIECE_NONE,
  FVW_PIECE_FIRE,
  FVW_PIECE_WATER,
  FVW_PIECE_ORIGIN,
  FVW_PIECE_MARKER,
  FVW_PIECE_VISITED,
} fvw_Piece;

typedef enum {
  FVW_PLAYER_FIRE,
  FVW_PLAYER_WATER
} fvw_Player;

typedef struct {
  size_t     dim;
  size_t     size;
  fvw_Piece* pieces;
} fvw_GameBoard;

FVW_BOOL fvw_IsWithinBounds(fvw_GameBoard* board, int x, int y);

FVW_BOOL fvw_IsOutsideBounds(fvw_GameBoard* board, int x, int y);

fvw_GameBoard* fvw_CreateBoard(size_t dimension);

void fvw_DestroyBoard(fvw_GameBoard** board);

void fvw_ClearBoard(fvw_GameBoard* board);

void fvw_ResetPieces(fvw_GameBoard* board);

FVW_BOOL fvw_GetPiece(fvw_GameBoard* board,
                      int x, int y,
                      fvw_Piece* piece);

FVW_BOOL fvw_SetPiece(fvw_GameBoard* board,
                      int x, int y,
                      fvw_Piece piece);

FVW_BOOL fvw_MovePiece(fvw_GameBoard* board,
                       int src_x, int src_y,
                       int dst_x, int dst_y );

FVW_BOOL fvw_MarkMoves(fvw_GameBoard* board,
                       int x, int y,
                       fvw_GameBoard* result);

#endif
