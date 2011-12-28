/**
 * Fire vs Water : Game rendering library
 *
 * This source file implements the functionality behind the
 * game's rendering routines.
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

#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "fvw_debug.h"
#include "fvw_render.h"

#ifdef FVW_EMBED_RES

#include "fvw_resources.h"

inline SDL_Surface* _fvw_LoadImageFromMem(void* ptr, size_t len, const char* type) {
    // memory for 'res' is freed by call to IMG_LoadTyped_RW
    return IMG_LoadTyped_RW(SDL_RWFromMem(ptr, len), 1, (char*)type);
}

#else

static const char* const FVW_FIRE_BOARD_ASSET_PATH  = "res/fire_board.png";
static const char* const FVW_WATER_BOARD_ASSET_PATH = "res/water_board.png";
static const char* const FVW_SRC_CURSOR_ASSET_PATH  = "res/src_cursor.png";
static const char* const FVW_DST_CURSOR_ASSET_PATH  = "res/dst_cursor.png";
static const char* const FVW_FIRE_PIECE_ASSET_PATH  = "res/fire_piece.png";
static const char* const FVW_WATER_PIECE_ASSET_PATH = "res/water_piece.png";
static const char* const FVW_FIRE_WINS_ASSET_PATH   = "res/fire_wins.png";
static const char* const FVW_WATER_WINS_ASSET_PATH  = "res/water_wins.png";

#endif

#define FVW_GET_PIECE(board, x, y)  board->pieces[(y * board->dim + x)]

FVW_BOOL fvw_LoadAssets(fvw_RendererAssets* assets) {
    TRACE_FN();

    ASSERT(assets != NULL);

    FVW_BOOL ret = FVW_FALSE;

    memset(assets, 0, sizeof(fvw_RendererAssets));

#ifdef FVW_EMBED_RES

    assets->fire_board =
        _fvw_LoadImageFromMem( fire_board_png, fire_board_png_len, "PNG" );

    assets->water_board =
        _fvw_LoadImageFromMem( water_board_png, water_board_png_len, "PNG" );

    assets->src_cursor =
        _fvw_LoadImageFromMem( src_cursor_png, src_cursor_png_len, "PNG" );

    assets->dst_cursor =
        _fvw_LoadImageFromMem( dst_cursor_png, dst_cursor_png_len, "PNG" );

    assets->fire_piece =
        _fvw_LoadImageFromMem( fire_piece_png, fire_piece_png_len, "PNG" );

    assets->water_piece =
        _fvw_LoadImageFromMem( water_piece_png, water_piece_png_len, "PNG" );

    assets->fire_wins =
        _fvw_LoadImageFromMem( fire_wins_png, fire_wins_png_len, "PNG" );

    assets->water_wins =
        _fvw_LoadImageFromMem( water_wins_png, water_wins_png_len, "PNG" );

#else

    assets->fire_board  = IMG_Load(FVW_FIRE_BOARD_ASSET_PATH);
    assets->water_board = IMG_Load(FVW_WATER_BOARD_ASSET_PATH);
    assets->src_cursor  = IMG_Load(FVW_SRC_CURSOR_ASSET_PATH);
    assets->dst_cursor  = IMG_Load(FVW_DST_CURSOR_ASSET_PATH);
    assets->fire_piece  = IMG_Load(FVW_FIRE_PIECE_ASSET_PATH);
    assets->water_piece = IMG_Load(FVW_WATER_PIECE_ASSET_PATH);
    assets->fire_wins   = IMG_Load(FVW_FIRE_WINS_ASSET_PATH);
    assets->water_wins  = IMG_Load(FVW_WATER_WINS_ASSET_PATH);

#endif

    // TODO: I need to be more clever about this...

    if ( !assets->fire_board  ||
         !assets->water_board ||
         !assets->src_cursor  ||
         !assets->dst_cursor  ||
         !assets->fire_piece  ||
         !assets->water_piece ||
         !assets->fire_wins   ||
         !assets->water_wins  ) {

        fvw_FreeAssets(assets);

        TRACE("assets failed to load!");
    } else {
        TRACE("assets loaded successfully.");
        ret = FVW_TRUE;
    }

    return ret;
}

void fvw_FreeAssets(fvw_RendererAssets* assets) {
    TRACE_FN();

    ASSERT(assets != NULL);

    if ( assets->fire_board ) SDL_FreeSurface(assets->fire_board);
    if ( assets->water_board ) SDL_FreeSurface(assets->water_board);
    if ( assets->src_cursor ) SDL_FreeSurface(assets->src_cursor);
    if ( assets->dst_cursor ) SDL_FreeSurface(assets->dst_cursor);
    if ( assets->fire_piece ) SDL_FreeSurface(assets->fire_piece);
    if ( assets->water_piece ) SDL_FreeSurface(assets->water_piece);
    if ( assets->fire_wins ) SDL_FreeSurface(assets->fire_wins);
    if ( assets->water_wins ) SDL_FreeSurface(assets->water_wins);

    memset(assets, 0, sizeof(fvw_RendererAssets));

    TRACE("assets freed.");
}

void fvw_RenderBoard( fvw_GameBoard*        board,
                      fvw_RendererSettings* settings,
                      fvw_RendererAssets*   assets,
                      SDL_Surface*          screen ) {

    TRACE_FN();

    ASSERT(board != NULL &&
           settings != NULL &&
           assets != NULL &&
           screen != NULL);

    /**
     * NOTE: This rapid lookup table must enjoy a 1-to-1 mapping to the FVW_PIECE
     * enumeration to work! Beats writing a big ugly switch statement :]
     */
    fvw_RendererAsset lookup[] = {
        NULL,
        assets->fire_piece,
        assets->water_piece,
        assets->src_cursor,
        assets->dst_cursor,
        NULL
    };

    TRACE("renderer offsets are:");
    VTRACE("... board-horiz=%d, board-vert=%d",
            settings->board_offset_h,
            settings->board_offset_v);
    VTRACE("... piece-horiz=%d, piece-vert=%d",
            settings->piece_offset_h,
            settings->piece_offset_v);

    int bx, by;
    SDL_Rect dst = {};
    fvw_RendererAsset asset = NULL;

    for ( by = 0; by < board->dim; ++by ) {
        for ( bx = 0; bx < board->dim; ++bx ) {
            if ( (asset = lookup[FVW_GET_PIECE(board, bx, by)])
                    != NULL ) {

                dst.x = (settings->board_offset_h +
                            ((bx - by) * settings->piece_offset_h))
                                - (asset->w >> 1);

                dst.y = (settings->board_offset_v +
                            ((bx + by) * settings->piece_offset_v))
                                - (asset->h >> 1);

                SDL_BlitSurface(asset, NULL, screen, &dst);

                VTRACE("rendered piece at (%d, %d) at (%d, %d)",
                        bx, by, dst.x, dst.y);
            }
        }
    }
}

void fvw_RenderAsset( fvw_GameBoard* board, int x, int y,
                      fvw_RendererSettings* settings,
                      fvw_RendererAsset     asset,
                      SDL_Surface*          screen ) {

    TRACE_FN();

    ASSERT(settings != NULL &&
           asset != NULL &&
           screen != NULL);

    if ( fvw_IsWithinBounds(board, x, y) ) {
        SDL_Rect dst = {
                (settings->board_offset_h +
                    ((x - y) * settings->piece_offset_h))
                        - (asset->w >> 1),
                (settings->board_offset_v +
                    ((x + y) * settings->piece_offset_v))
                        - (asset->h >> 1)
            };

        SDL_BlitSurface(asset, NULL, screen, &dst);
    }
}

void fvw_RenderPlayerWin( fvw_Player          player, 
                          fvw_RendererAssets* assets,
                          SDL_Surface*        screen) {

    TRACE_FN();

    ASSERT(assets != NULL);
    ASSERT(screen != NULL);

    fvw_RendererAsset asset = player == FVW_PLAYER_FIRE ?
        assets->fire_wins : assets->water_wins;

    SDL_Rect dst = {
            (screen->w >> 1) - (asset->w >> 1),
            (screen->h >> 1) - (asset->h >> 1)
        };

    SDL_BlitSurface(asset, NULL, screen, &dst);
}

