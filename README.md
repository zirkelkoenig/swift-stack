# Play Instructions
The goal of the game is to reach level 999, with each piece placement and each line cleared counting as one level. Each
100 levels make up a section. To cross a section boundary (for example to go from level 199 to 200), you have to clear
at least one line (the same applies to going from level 998 to 999).

You can initiate fast shift by holding down one of the shift keys for a short period of time.

When a piece hits the ground or the top of the stack, you have a short amount of time to further move it before it
locks. A movement that results in the piece moving down resets that time.

A soft drop moves the piece down with high speed, locking it instantly as soon as it hits the ground or the top of the
stack.

A sonic drop instantly drops the piece to the ground or the top of the stack without locking it.

During the time between the last piece locking and the next piece spawning, you can buffer rotations by holding down one
or more rotation keys. The piece then spawns pre-rotated. You can also use this time frame to pre-load the fast shift.

The game is over when you reach level 999 or when a new, possibly pre-rotated piece can not be spawned without
collisions.

## Controls

* S - Shift left
* F - Shift right
* D - Soft drop

* J - Rotate counter-clockwise
* K - Rotate clockwise
* L - Rotate counter-clockwise

* Space - Sonic drop

# Build Instructions
## Pre-Requisites

* SDL library (>= 2.0)
* SDL_ttf library (>= 2.0)
* CMake build system (>= 3.5)

## Linux
Install the SDL and SDL_ttf development libraries and the CMake build system via your package manager, then run `cmake`
and `make` to build the game.

To run, the directory `fonts` needs to be located in the same directory as the game executable. `make install` is not
implemented yet.

## Windows (Visual Studio)
Download and unpack the SDL and SDL_ttf development libraries, then download, install and run CMake. During
configuration, you'll have to manually set the library path variables as follows:

* `SDL2MAIN_LIBRARY = <SDL2 Path>/lib/<Platform>/SDL2main.lib`
* `SDL2_INCLUDE_DIR = <SDL2 Path>/include`
* `SDL2_LIBRARY_TEMP = <SDL2 Path>/lib/<Platform>/SDL2.lib`
* `SDL2TTF_INCLUDE_DIR = <SDL2_ttf Path>/include`
* `SDL2TTF_LIBRARY_TEMP = <SDL2_ttf Path>/lib/<Platform>/SDL2_ttf.lib`

Substitute `<SDL2 Path>` and `<SDL2_ttf Path>` for the paths where you unpacked the libraries to and substitute
`<Platform>` for either `x86` or `x64` for 32- and 64-Bit-Systems respectively.

Afterwards, a Visual Studio project is created at the path specified earlier which you can open and finally build after
setting `tetris` as the Startup Project in the Solution Explorer. After building, you have to manually copy

* `SDL2.dll`
* `SDL2_ttf.dll`
* `libfreetype-6.dll`
* `zlib1.dll`
* `fonts`-Folder

to the game executable's location.

# Background
I started this project to build a clone of the infamous "Tetris The Grandmaster" series by Arika that would run on a PC
and be played with the keyboard. As such, there are currently no plans to port the game to gaming consoles or mobile
devices. The latter seems pretty much impossible to me anyways, because of the game's speed and the precision of the
controls that it requires.

Currently, the internal timings reflect those of TGM2's Master Mode, sans the time requirements and invisible/fading
credit roll.

# Code Details
## Pieces
Internally, all pieces are identified by their color.

* I - Red
* T - Turquoise
* L - Orange
* J - Blue
* S - Pink
* Z - Green
* O - Yellow

## Phases

* LOADING: Delay before a new piece is spawned.
	1. Check shift inputs for fast shift charge
	2. Increase phase counter
	3. Check for phase end:
		1. Spawn new piece
		2. Perform initial rotation
		3. Check for block out

* DROPPING: Main phase, player can input all commands, piece is pulled down by gravity.
	1. Process rotation inputs
	2. Process (fast) shift inputs
	3. Apply gravity (check for phase change)
	4. Process soft/hard drop inputs (check piece locking with phase change)

* LOCKING: Delay before a piece locks, can be reset by a successful drop.
	1. Process rotation inputs
	2. Process (fast) shift inputs
	3. Apply maximum gravity (check for drop -> lock delay reset)
	4. Process soft drop input (lock piece)
	5. Increase phase counter (check for phase change)

* CLEARING: Line clear delay, player must wait
	1. Increase phase counter
	2. Check for phase change (increase level by lines destroyed);

# Code Remarks
## Coding Style
Indent with tabs (8 spaces wide), maximum line width is 120 characters. Indent style is K&R.

## Return codes
All functions with return codes should follow this guideline:

* -1 - Internal error, like a failed call to an external function
* -2 - Malformed arguments

Functions returning pointers return NULL on error.
