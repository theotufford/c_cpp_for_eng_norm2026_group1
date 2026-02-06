# C/C++ for eng norm2026 group 1
**git repo for the work of group 1 in CS1113 C/C++ for engineers at Normandale community college**

Our overarching goal is to make pvp wordle.

### project 1: base wordle engine

2 players, 2 boards

player:
player points : int

board:
board owner : player
board secret : string
board guess history : vector of strings

main loop:

if guess history empty,
prompt other player for secret word, pass turn back

take player guess 

check guess against secret word, 
on a correct guess give point to board owner and clear the guess history. then pass turn to other player

attach guess to guess history
use guess history to render wordle board
pass the turn to the other player and repeat all of this


