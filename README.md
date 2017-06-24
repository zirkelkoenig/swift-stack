# Play Instructions

* S - Move left
* F - Move right
* D - Soft drop
* E - Sonic drop

* J - Rotate counter-clockwise
* K - Rotate clockwise
* L - Rotate counter-clockwise

# Game Details
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
