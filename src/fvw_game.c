/**
 * Fire vs Water: Game library implementation
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "fvw_debug.h"
#include "fvw_core.h"
#include "fvw_render.h"
#include "fvw_game.h"

const char* const    FVW_GAME_WINDOW_TITLE    = "Fire vs. Water";
const size_t         FVW_GAME_BOARD_SIZE      = 9;
fvw_GameBoard*       FVW_GAME_BOARD           = NULL;
fvw_GameBoard*       FVW_GAME_MOVES           = NULL;
SDL_Surface*         FVW_GAME_SCREEN          = NULL;
fvw_RendererAssets   FVW_GAME_RENDER_ASSETS   = {};
fvw_RendererSettings FVW_GAME_RENDER_SETTINGS = { 400, 44, 32, 32 };
int                  FVW_GAME_CURSOR_X        = -1;
int                  FVW_GAME_CURSOR_Y        = -1;

fvw_GameBoard* fvw_GetGameBoard() {
    return FVW_GAME_BOARD;
}

fvw_RendererAssets* fvw_GetGameAssets() {
    return &FVW_GAME_RENDER_ASSETS;
}

SDL_Surface* fvw_GetGameScreen() {
    return FVW_GAME_SCREEN;
}

FVW_BOOL fvw_GameInit(int width, int height, Uint32 video_flags) {
    TRACE_FN();

    const Uint32 sdl_flags = SDL_INIT_VIDEO;
    const Uint32 img_flags = IMG_INIT_PNG;

    if ( SDL_WasInit(sdl_flags) != sdl_flags &&
            SDL_Init(sdl_flags) != 0 ) {

            fprintf(stderr, "Used SDL flags: 0x%x\n", sdl_flags);
            fvw_GameError("Failed to initialize Simple DirectMedia Layer.");
            return FVW_FALSE;
    }

    if ( IMG_Init(img_flags) != img_flags ) {
        fprintf(stderr, "Used SDL image flags: 0x%x\n", img_flags);
        fvw_GameError("Failed to initialize SDL image library.");
        return FVW_FALSE;
    }

    SDL_Surface* screen = SDL_SetVideoMode(width, height, 0, video_flags);
    if ( screen == NULL ) {
        fprintf(stderr, "Used SDL video flags: 0x%x\n", video_flags);
        fvw_GameError("Failed to set video mode.");
        return FVW_FALSE;
    }

    if ( fvw_LoadAssets(&FVW_GAME_RENDER_ASSETS) != FVW_TRUE ) {
        fvw_GameError("Failed to load game assets.");
        return FVW_FALSE;
    }

    if ( (FVW_GAME_BOARD = fvw_CreateBoard(FVW_GAME_BOARD_SIZE)) == NULL ) {
        fvw_GameError("Failed to create game board.");
        return FVW_FALSE;
    }

    if ( (FVW_GAME_MOVES = fvw_CreateBoard(FVW_GAME_BOARD_SIZE)) == NULL ) {
        fvw_GameError("Failed to create move scratch space.");
        return FVW_FALSE;
    }


    FVW_GAME_SCREEN = screen;
    SDL_WM_SetCaption(FVW_GAME_WINDOW_TITLE, NULL);

    VTRACE("game started at resolution %dx%d", width, height);
    TRACE("renderer offsets are:");
    VTRACE("... board-horiz=%d, board-vert=%d",
            FVW_GAME_RENDER_SETTINGS.board_offset_h,
            FVW_GAME_RENDER_SETTINGS.board_offset_v);
    VTRACE("... piece-horiz=%d, piece-vert=%d",
            FVW_GAME_RENDER_SETTINGS.piece_offset_h,
            FVW_GAME_RENDER_SETTINGS.piece_offset_v);

    return FVW_TRUE;
}

void fvw_GameReset() {
    TRACE_FN();
    ASSERT(FVW_GAME_BOARD != NULL);
    ASSERT(FVW_GAME_SCREEN != NULL);

    FVW_GAME_CURSOR_X = -1;
    FVW_GAME_CURSOR_Y = -1;

    fvw_ResetPieces(FVW_GAME_BOARD);
}

void fvw_GameCleanUp() {
    TRACE_FN();

    if ( FVW_GAME_MOVES != NULL ) {
        fvw_DestroyBoard(&FVW_GAME_MOVES);
        TRACE("game move scratch space destroyed");
    }

    if ( FVW_GAME_BOARD != NULL )
        fvw_DestroyBoard(&FVW_GAME_BOARD);

    fvw_FreeAssets(&FVW_GAME_RENDER_ASSETS);

    if ( FVW_GAME_SCREEN != NULL ) {
        SDL_FreeSurface(FVW_GAME_SCREEN);
        TRACE("video surface released");
        FVW_GAME_SCREEN = NULL;
    }
}

void fvw_GameExit(int status) {
    TRACE_FN();
    fvw_GameCleanUp();
    IMG_Quit();
    SDL_Quit();
    VTRACE("exiting with status %d", status);
    exit(status);
}

void fvw_GameError(const char* message) {
    TRACE_FN();
    if (message == NULL) return;
    fputs("[ERR] ", stderr);
    fputs(message, stderr);
    fputs("\n", stderr);
}

void fvw_GameAbort(const char* message) {
    TRACE_FN();
    if (message != NULL)
        fvw_GameError(message);
    fvw_GameExit(EXIT_FAILURE);
}

void fvw_GameScreenClear(Uint8 r, Uint8 g, Uint8 b) {
    TRACE_FN();
    ASSERT(FVW_GAME_SCREEN != NULL);

    SDL_FillRect(FVW_GAME_SCREEN, NULL,
            SDL_MapRGB(FVW_GAME_SCREEN->format, r, g, b));
}

void fvw_GameScreenRedraw(fvw_Player player, int show_moves) {
    TRACE_FN();

    ASSERT(FVW_GAME_SCREEN != NULL);
    ASSERT(FVW_GAME_RENDER_ASSETS.fire_board != NULL);
    ASSERT(FVW_GAME_RENDER_ASSETS.water_board != NULL);
    ASSERT(FVW_GAME_RENDER_ASSETS.src_cursor != NULL);

    fvw_RendererAsset board_asset = player == FVW_PLAYER_FIRE ?
                                        FVW_GAME_RENDER_ASSETS.fire_board :
                                        FVW_GAME_RENDER_ASSETS.water_board;

    SDL_BlitSurface(
            board_asset,
            NULL,
            FVW_GAME_SCREEN,
            NULL);

    fvw_RenderBoard(
            FVW_GAME_BOARD,
            &FVW_GAME_RENDER_SETTINGS,
            &FVW_GAME_RENDER_ASSETS,
            FVW_GAME_SCREEN);

    if ( fvw_GameCursorIsSet() ) {
        fvw_RenderAsset(
                FVW_GAME_BOARD,
                FVW_GAME_CURSOR_X,
                FVW_GAME_CURSOR_Y,
                &FVW_GAME_RENDER_SETTINGS,
                FVW_GAME_RENDER_ASSETS.src_cursor,
                FVW_GAME_SCREEN);

        if ( show_moves &&
            fvw_MarkMoves(
                    FVW_GAME_BOARD,
                    FVW_GAME_CURSOR_X,
                    FVW_GAME_CURSOR_Y,
                    FVW_GAME_MOVES) ) {
            fvw_RenderBoard(
                    FVW_GAME_MOVES,
                    &FVW_GAME_RENDER_SETTINGS,
                    &FVW_GAME_RENDER_ASSETS,
                    FVW_GAME_SCREEN);
       }
    }
}

void fvw_GameScreenRefresh() {
    TRACE_FN();
    ASSERT(FVW_GAME_SCREEN != NULL);
    SDL_Flip(FVW_GAME_SCREEN);
}

void fvw_GameCursorSet(int x, int y) {
    if ( fvw_IsWithinBounds(FVW_GAME_BOARD, x, y) ) {
        FVW_GAME_CURSOR_X = x;
        FVW_GAME_CURSOR_Y = y;
    } else {
        VTRACE("could not set game cursor at (%d, %d)", x, y);
    }
}

void fvw_GameCursorUnset() {
    FVW_GAME_CURSOR_X = FVW_GAME_CURSOR_Y = -1;
}

int fvw_GameCursorIsSet() {
    return (FVW_GAME_CURSOR_X == -1 && FVW_GAME_CURSOR_Y == -1) ? 0 : 1;
}

FVW_BOOL fvw_GameMovePieceUnderCursor(int dst_x, int dst_y) {
    TRACE_FN();
    ASSERT(FVW_GAME_BOARD != NULL);

    FVW_BOOL ret = FVW_FALSE;

    fvw_Piece piece;

    if ( fvw_GameCursorIsSet() &&
             fvw_GetPiece(FVW_GAME_BOARD, dst_x, dst_y, &piece) &&
                piece == FVW_PIECE_NONE ) {

        VTRACE("trying to move (%d, %d) to (%d, %d)",
                FVW_GAME_CURSOR_X, FVW_GAME_CURSOR_X,
                dst_x, dst_y );

        if ( fvw_MarkMoves(
                  FVW_GAME_BOARD,
                  FVW_GAME_CURSOR_X,
                  FVW_GAME_CURSOR_Y,
                  FVW_GAME_MOVES) &&
                fvw_GetPiece(FVW_GAME_MOVES, dst_x, dst_y, &piece) &&
                    piece == FVW_PIECE_MARKER &&
             fvw_MovePiece(
                     FVW_GAME_BOARD,
                     FVW_GAME_CURSOR_X,
                     FVW_GAME_CURSOR_Y,
                     dst_x,
                     dst_y) ) {
            TRACE("moved successfully");

            fvw_GameCursorUnset();

            ret = FVW_TRUE;
        } else {
            TRACE("failed to move cursor");
        }
    } else {
        VTRACE("cannot move piece under cursor to (%d, %d)", dst_x, dst_y);
    }

    return ret;
}

FVW_BOOL fvw_GameCheckForWinner(fvw_Player* player) {
    TRACE_FN();

    ASSERT(FVW_GAME_BOARD != NULL);
    ASSERT(player != NULL);

    FVW_BOOL ret = FVW_FALSE;

    int water = 1, fire = 1;
    int x, y, dim = (FVW_GAME_BOARD->dim >> 1);
    fvw_Piece pc;

    for ( y = 0; y < dim; ++y ) {
        for ( x = 0; x < (dim - y); ++x ) {
            if ( fire &&
                    fvw_GetPiece(FVW_GAME_BOARD, x, y, &pc) &&
                        pc != FVW_PIECE_FIRE )
                fire = 0;

            if ( water &&
                    fvw_GetPiece(
                        FVW_GAME_BOARD,
                        FVW_GAME_BOARD->dim - x - 1,
                        FVW_GAME_BOARD->dim - y - 1,
                        &pc) &&
                    pc != FVW_PIECE_WATER )
                water = 0;

            if ( !fire && !water )
                return FVW_FALSE;
        }
    }

    if ( fire && water ) {
        fvw_GameError("Two winners.");
        ret = FVW_FALSE;
    } else if ( fire || water ) {
        *player = fire ? FVW_PLAYER_FIRE : FVW_PLAYER_WATER;
        ret = FVW_TRUE;
    }

    return ret;
}

FVW_BOOL fvw_GameResolvePoint(float tol, int sx, int sy, int* rx, int* ry) {
    TRACE_FN();

    ASSERT(rx != NULL && ry != NULL);
    ASSERT(tol >= 0.0);

    FVW_BOOL ret = FVW_FALSE;

    // normalized coordinates
    float nx = sx - FVW_GAME_RENDER_SETTINGS.board_offset_h,
          ny = sy - FVW_GAME_RENDER_SETTINGS.board_offset_v
                        + (FVW_GAME_RENDER_SETTINGS.piece_offset_v >> 2);

    // approximate board coordinates
    float bx = (nx + ny) /
            (float)(FVW_GAME_RENDER_SETTINGS.piece_offset_h << 1);
    float by = (ny - nx) /
            (float)(FVW_GAME_RENDER_SETTINGS.piece_offset_v << 1);

    // make sure it's in the game board area...
    if ( fvw_IsWithinBounds(FVW_GAME_BOARD, bx, by) ) {

        // limit based on tolerances
        if ( fmod(bx, 1) <= tol &&
             fmod(by, 1) <= tol ) {

            *rx = (int)bx;
            *ry = (int)by;
            ret = FVW_TRUE;
        }
    }

    return ret;
}

