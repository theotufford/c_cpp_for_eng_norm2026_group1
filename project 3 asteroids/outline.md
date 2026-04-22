# Project 3 - Asteroids
## Outline 
At the lowest level we need to do a few things:
1. create asteroids
2. control the player
3. collision check game objects
4. evolve objects in space given their velocity vector
5. render the game state
### Player Outline
**methods**
- shoot bullets forward
- Collision check
**properties**
- lives
- position
- current angle
- angular acceleration
- linear acceleration
- accelerating linear
- accelerating angular
    - ^ values should be -1 0 or 1. this is set by the control input, actual velocity change is handled in the game loop
- angular velocity
- forward velocity
- max angular velocity
- max linear velocity
- hit radius
- sprite 
### Bullet Outline
**methods**
- Collision check
**properties**
- position
- forward normal 
- forward velocity
- hit radius
### Asteroid Outline
**methods**
- split
- Collision check
**properties**
- size
    - Size 5 asteroid turns into two size 4 asteroids upon being killed. So on and so forth.
- position
- forward velocity
- current angle
- forward normal
- hit radius
- sprite
### Game Outline
**main loop**
- get tick time delta
    - elapsed time from last loop tick 
- spawn asteroids
- check for control input
- evolve velocity from acceleration
- evolve positions from velocity
- collision check all objects
- handle collision side effects - defined by implementation of collision check function
    - bullet collides with asteroid -> +score, destroy bullet, split if size > 0, else destroy
    - asteroid collides with player -> -life (game over if 0), destroy asteroid
    - asteroid collides with asteroid -> nothing
- render game
**globals**
- tick time delta
    - used in velocity and position calculations
- score
- vector of pointers to game objects
    - used for collision checking and rendering
- player object























