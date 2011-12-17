/**
 * Fire vs Water : Main game application
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
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "fvw_game.h"

#define TOGGLE(val) val = val == 0 ? 1 : 0;

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if ( !fvw_GameInit(800, 600, SDL_HWSURFACE) )
        fvw_GameExit(EXIT_FAILURE);

    int bx, by;

    int listen     = 1,
        redraw     = 1,
        mode1337   = 0,
        player_win = 0;

    fvw_Piece piece;

    fvw_Player winner, player = (rand() % 100) % 2 == 0 ?
        FVW_PLAYER_FIRE : FVW_PLAYER_WATER;

    SDL_Event event;

    fvw_GameReset();

    while ( listen && !player_win ) {
        if ( redraw != 0 ) {
            fvw_GameScreenClear(0, 0, 0);

            fvw_GameScreenRedraw(player, mode1337);

            fvw_GameScreenRefresh();

            redraw = 0;
        }

        if ( !SDL_WaitEvent(&event) ) break;

        switch ( event.type ) {
        case SDL_MOUSEBUTTONDOWN:
            if ( fvw_GameResolvePoint(
                        0.3f,
                        event.button.x, event.button.y,
                        &bx, &by) ) {

                fvw_GetPiece(fvw_GetGameBoard(), bx, by, &piece);

                if ( (piece == FVW_PIECE_FIRE &&
                        player == FVW_PLAYER_FIRE) ||
                     (piece == FVW_PIECE_WATER &&
                        player == FVW_PLAYER_WATER) ) {
                    fvw_GameCursorSet(bx, by);
                    redraw = 1;
                } else if ( fvw_GameCursorIsSet() &&
                             piece == FVW_PIECE_NONE &&
                             fvw_GameMovePieceUnderCursor(bx, by) ) {
                    player = player == FVW_PLAYER_FIRE ?
                                FVW_PLAYER_WATER : FVW_PLAYER_FIRE;

                    if ( fvw_GameCheckForWinner(&winner) )
                        player_win = 1;
                    redraw = 1;
                }
            }
            break;
        case SDL_KEYDOWN:
            if ( event.key.keysym.mod == KMOD_LSHIFT &&
                    event.key.keysym.sym == (SDLK_a | SDLK_w) )
                redraw = 1;
                TOGGLE(mode1337);
            break;
        case SDL_QUIT:
            listen = 0;
            break;
        }
    }

    if ( player_win ) {
        fvw_GameScreenRedraw(player, mode1337);
        fvw_RenderPlayerWin(winner, fvw_GetGameAssets(), fvw_GetGameScreen());
        fvw_GameScreenRefresh();

        // wait for keypress / quit for player-win screen exit
        while ( SDL_WaitEvent(&event) &&
                    (event.type != SDL_KEYDOWN &&
                     event.type != SDL_QUIT) );
    }

    fvw_GameExit(EXIT_SUCCESS);

    return 0;
}

