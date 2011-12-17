/**
 * Fire vs Water : Renderer
 *
 * Interface for rendering game objects.
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

#ifndef FVW_RENDER_H
#define FVW_RENDER_H 

#include "fvw_core.h"

struct SDL_Surface;

typedef SDL_Surface* fvw_RendererAsset;

typedef struct {
  fvw_RendererAsset fire_board;
  fvw_RendererAsset water_board;
  fvw_RendererAsset src_cursor;
  fvw_RendererAsset dst_cursor;
  fvw_RendererAsset fire_piece;
  fvw_RendererAsset water_piece;
  fvw_RendererAsset fire_wins;
  fvw_RendererAsset water_wins;
} fvw_RendererAssets;

typedef struct {
  int board_offset_h;
  int board_offset_v;
  int piece_offset_h;
  int piece_offset_v;
} fvw_RendererSettings;

FVW_BOOL fvw_LoadAssets(fvw_RendererAssets* assets);

void fvw_FreeAssets(fvw_RendererAssets* assets);

void fvw_RenderBoard(fvw_GameBoard*        board,
                     fvw_RendererSettings* settings,
                     fvw_RendererAssets*   assets,
                     SDL_Surface*          screen );

void fvw_RenderAsset(fvw_GameBoard* board, int x, int y,
                     fvw_RendererSettings* settings,
                     fvw_RendererAsset     asset,
                     SDL_Surface*          screen );

void fvw_RenderPlayerWin(fvw_Player          player, 
                         fvw_RendererAssets* assets,
                         SDL_Surface*        screen );

#endif

