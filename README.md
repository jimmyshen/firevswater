WHAT IT IS
==========

Fire vs. Water&mdash;or "fvw" for short&mdash;is a themed Chinese Checkers game
designed for two players. The game was borne from a few enjoyable
rounds of the Chinese Checkers with my girlfriend. I decided to create a
software version since I was itching to write a game. The software
version is also decidedly easy to travel with. We played this on our
way to Vegas! :)

To make it more interesting, I've designed the graphics for the game
based on a graphical theme of fire versus water. As you can see, I was
not as creative with the name.

I originally had plans to extend the game to support network play, but
have lost interest in developing it. I've decided to introduce this
source code to the wild--blemishes and all.

Enjoy!

HOW TO PLAY
===========

The game relies solely on the mouse for play. Simply select a piece
and click where you want it to go. If the move is legal, the piece
will move. Then it becomes the next player's turn to move. This continues
until one player manages to move all their pieces into their opponent's
corner.

A certain three-key combination will allow you to enable a mode that
reveals all possible moves for a highlighted piece.

LIMITATIONS
===========

At the moment, this game:

- Only runs in 800x600, windowed mode. A one-ine change in the source
  code will allow you to run it in fullscreen.

- At the moment, there is no way for you to exit the game except by
  clicking your system's native window exit control. If you choose to
  go fullscreen, you may want to add a keybinding into the code.

- After a player wins, the game will wait for one more keypress before
  exiting completely. Thus, there is no way to "Play Again" within the
  game.

- When built without the FVW\_EMBED\_RES option, the game expects to find
  all image resources in the 'res' folder.

HOW TO BUILD
============

Dependencies
------------

This source has no other dependencies other than SDL and SDL\_image.
Thus, you should not have much trouble building this software on
platforms supported by SDL and SDL\_image. You can download SDL
and SDL\_image from the web:

- http://www.libsdl.org
- http://www.libsdl.org/projects/SDL\_image

The supplied Makefile is designed for \*NIX style systems, so
targeting such a machine would be the easiest route.

Debug build
-----------

I added a bunch of debug logging that can be enabled by
specifying FVW\_DEBUG=1 when running make:

    % make FVW_DEBUG=1

This will also add the -g flag to the compilation for including
debug symbols in the binary.

Embedded resources
------------------

You can specify the FVW\_EMBED\_RES option to tell make to
generate headers that encode all the image resources for
compilation into the final binary:

    % make FVW_EMBED_RES

LICENSE
=======

This source code is distributed under the GNU General Public License.
Please see COPYING for more information.

CREDITS
=======

SDL and SDL\_image library courtesy of Sam Latinga
GNU Library General Public License
Copyright (c) 1997-2009
http://www.libsdl.org

Marble graphics courtesy of DragonArtz
Creative Commons Attribution Noncommercial Share-Alike 3.0
http://dragonartz.wordpress.com

All other graphics and programming by Chun Min Shen
These resources are
