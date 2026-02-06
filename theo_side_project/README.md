# random musings and odds and ends etc.
## terminal game engine
Only works on linux I havent figured out input on mac or windows and I dont use a mac or windows device so it would be hard to test.
Really inefficient but getting better. 
Essentially the pipeline is this: 
    1. check for keyboard input
    2. if keyboard input set delta vector according to direction
    3. set previous position = current position
    4. transform player game object by + delta vector
    5. if no keyboard input continue the loop without the rest of this
    6. else 
    7. iterate through all of the objects in the game
        1. pre-render object by iterating through model points
            1. check unordered set of points that corresponds to the points already activated during this rending cycle to see if this point has already been activated
            2. if found trigger the collision methods of both the current object and the collided object
        2. if not found add this point to render cycle history
        3. set integer map coordinate of this point to 1 (other values may later represent unique coloring but for now 1 is active and 0 is background)
        4. 
