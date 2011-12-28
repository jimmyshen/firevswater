What it is
==========

Fire vs. Water--or "fvw" for short--is a themed Chinese Checkers game
designed for two players. The game was borne from a few enjoyable rounds
of Chinese Checkers with my girlfriend. I decided to create a software
version since I was itching to write a game. Also, the software version is
decidedly easier to travel with. We played this on our way to Vegas! :)

To make it more interesting, I've designed the game based on a graphical
theme of fire versus water. As you can see, I was not as creative with
the title of the game.

I originally had plans to extend the game to support network play, but
have lost interest in developing it. I've decided to introduce this
source code to the wild--blemishes and all.

Enjoy!

How to play
===========

The game relies solely on the mouse for play. Simply select a piece
and click where you want it to go. If the move is legal, the piece
will move. Then it becomes the next player's turn to move. This continues
until one player manages to move all their pieces into their opponent's
corner.

A certain three-key combination will allow you to enable a mode that
reveals all possible moves for a highlighted piece.

Press 'q' or <ESC> to exit the game.

Screenshot
==========

![fvw screenshot](http://i.imgur.com/jnrIp.png)

Limitations
===========

At the moment, this game:

- Only runs in 800x600, windowed mode. A one-ine change in the source
  code will allow you to run it in fullscreen.

- After a player wins, the game will wait for one more keypress before
  exiting completely. Thus, there is no way to "Play Again" within the
  game.

- When built without the FVW_EMBED_RES option, the game expects to find
  all image resources in the 'res' folder.

How to build
============

Dependencies
------------

This source has no other dependencies other than SDL and SDL_image.
Thus, you should not have much trouble building this software on
platforms supported by SDL and SDL_image. You can download SDL
and SDL_image from the web:

- http://www.libsdl.org
- http://www.libsdl.org/projects/SDL_image

The supplied Makefile is designed for *NIX style systems, so
targeting such a machine would be the easiest route.

Debug build
-----------

I added a bunch of debug logging that can be enabled by
specifying FVW_DEBUG=1 when running make:

    % make FVW_DEBUG=1

This will also add the -g flag to the compilation for including
debug symbols in the binary.

Embedded resources
------------------

You can specify the FVW_EMBED_RES option to tell make to
generate headers that encode all the image resources for
compilation into the final binary:

    % make FVW_EMBED_RES

License
=======

This source code is distributed under the GNU General Public License.
Please see COPYING for more information.

Credits
=======

SDL and SDL_image library courtesy of Sam Latinga
GNU Library General Public License
Copyright (c) 1997-2009
http://www.libsdl.org

Marble graphics courtesy of DragonArtz
Creative Commons Attribution Noncommercial Share-Alike 3.0
http://dragonartz.wordpress.com

All other graphics and programming by Chun Min Shen
Copyright (c) 2011
