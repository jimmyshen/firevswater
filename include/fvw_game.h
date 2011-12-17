/**
 * Fire vs Water : Game library
 *
 * This header provides an interface to common game functions to be used
 * to build the main game application.
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

#ifndef FVW_GAME_H
#define FVW_GAME_H

#include "fvw_core.h"
#include "fvw_render.h"

struct SDL_Surface;

fvw_GameBoard* fvw_GetGameBoard();

fvw_RendererAssets* fvw_GetGameAssets();

SDL_Surface* fvw_GetGameScreen();

FVW_BOOL fvw_GameInit(int width, int height, Uint32 video_flags);

void fvw_GameReset();

void fvw_GameExit(int status);

void fvw_GameCleanUp();

void fvw_GameError(const char* message);

void fvw_GameAbort(const char* message);

void fvw_GameScreenClear(Uint8 r, Uint8 g, Uint8 b);

void fvw_GameScreenRedraw(fvw_Player player, int show_moves);

void fvw_GameScreenRefresh();

void fvw_GameCursorSet(int x, int y);

void fvw_GameCursorUnset();

int fvw_GameCursorIsSet();

FVW_BOOL fvw_GameMovePieceUnderCursor(int dst_x, int dst_y);

FVW_BOOL fvw_GameResolvePoint(float tol, int sx, int sy, int* rx, int* ry);

FVW_BOOL fvw_GameCheckForWinner(fvw_Player* player);

#endif
