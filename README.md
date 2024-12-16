# Special Chess Variant Rules

## Game Overview
This is a two-player turn-based board game that combines elements of traditional chess with unique terrain systems and special abilities mechanics.

## Game Board
* 11x11 board
* Features multiple special terrain types:
  * Land: Basic terrain with no special effects
  * River: Located in the center of the board, certain pieces cannot cross
  * Forest: Restricts movement distance and patterns
  * Mountain: Limits movement distance
  * Desert: Prohibits piece capture
  

## Piece Types
Each player has 11 pieces:
* 2 Knights (Blue)
* 2 Bishops (Cyan)
* 2 Pawns (Green)
* 1 Bomb (Red)
* 1 Queen (Magenta)
* 1 King (Yellow)
* 2 Additional Pawns (Green)

## Basic Movement Rules

### Knight
* Can move up to 2 squares in straight lines
* Can move 1 square diagonally plus 1 square straight

### Pawn
* Can only move 1 square horizontally or vertically
* Has no special ability

### Bomb
* Can only move 1 square horizontally or vertically
* Cannot cross rivers

### Queen
* Can move up to 2 squares in straight lines or diagonally
* Cannot cross rivers

### King
* Can move 1 square in any direction
* Cannot cross rivers

### Bishop
* Can move up to 2 squares diagonally
* Cannot cross rivers

## Special Abilities

### Knight's Special Ability
* Can charge forward up to 5 squares, and will not go out of the scene
* Can capture an enemy piece and stop at its position
* Stops before friendly pieces
* Can only be used once per game

### Bomb's Special Ability
* When detonated, destroys all pieces (including itself) in a 3x3 area

### Queen's Special Ability
* Can eliminate enemy pieces at the four corners of a 4x4 square centered on herself

### King's Special Ability
* Can swap positions with the nearest friendly Knight
* Can only be used once per game

### Bishop's Special Ability
* Can spawn a friendly Pawn in front of itself
* Can be used twice per game

## Terrain Effects

### Forest
* Movement restrictions:
  * Maximum 2 squares in straight lines
  * Or maximum 2 squares diagonally

### Mountain
* Restricts pieces to moving only 1 square horizontally or vertically

### Desert
* Prohibits piece capture in this terrain

### River
* Bombs, Queens, Kings, and Bishops cannot cross
* Other pieces can cross normally

## Game Rules

### Turn System
* Players alternate turns
* Each turn allows one piece movement or special ability use

### Capture Rules
* Moving to an enemy piece's position captures it
* Cannot capture friendly pieces
* No captures allowed on desert terrain
* Special cases:
  * When a Bomb is captured, it explodes and eliminates the capturing piece
  * When a Bomb captures another piece, the Bomb also disappears
  * When two Bombs interact, both are eliminated

### Victory Conditions
* Capture the opponent's King
* Or eliminate all opponent's pieces

### Additional Rules
* Pieces cannot jump over other pieces (unless allowed by special abilities)
* Using a special ability ends the current turn
* Each piece's error messages will indicate invalid movements (e.g., trying to cross rivers with restricted pieces)