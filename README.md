# CS300 Final Project: The Kingdom of Frupal
## Group 5:
- Max Carlson, Scrum Master
- Darian Hutchinson
- Sam Rind
- Devon Berry
- Courtney Ficker
---

### Instructions
- To run the game, compile by typing **make**, then run by typing **./frupal**

- Example Map:
> At least one map file for your game. This map should be winnable and demonstrate all the features of the game * (confused) -- **TODO: I think we need a link to a save file (like mapSave_1.txt) that we know is winnable -Courtney**

- Playing the game:

    When the game loads, choose one of the following options at the main menu:
    
      1. Start The Game
      2. Select Map Seed
      3. Quit


    Option 2 will allow you to enter an integer that will set the seed to randomize the game. This allows you to replay certain maps if desired.

    Option 1 loads the game proper, and gives the player two sets of controls:
    1. The cursor, controlled with numbers 1-4 (controls are printed on-screen)
    2. The player itself, controllable with arrow keys

    The player and cursor are totally independent of each other. The cursor will highlight map spaces and information will be printed out on the upper-right of the screen (or in a centered window in the case of Clues), provided the map space has already been "discovered" by the player.
    
    The player must move to spaces in order to interact with the items there. Upon running into an occupied space, different things may happen:
    - For tools and food, the player is prompted to purchase them for a displayed price. Food is immediately consumed and the associated energy boost applied to their energy meter. Tools are held by the player to be used later, against obstacles.
    - Obstacles will allow players to attempt to break it with their fists, in which case the player is sapped of the listed energy cost, potentially killing them. Players can also choose to use a tool, which will reduce the amount of energy required by a variable degree (2x, 3x, 4x, etc.). Once the obstacle is destroyed, the player may pass freely.
    - Chests are immediately picked up and their contents added to the players funds. The royal diamonds will immediately win the game for the player.
    - Ships will allow the player to traverse water spaces for free, getting left behind on the shore when the player disembarks.


### Discussion:

We were given three Stretch Goals as part of the instructions for this project, and were able to achieve all of them.

1. Random map generation came first, thanks to Max. Having what was likely the hardest stretch goal out of the way right off the bat was a huge help, but it also made map saving and loading (the 3rd stretch goal) more challenging.
2. Item loading was accomplished later. It allows a person to edit a text file to add new Tools, Food, and Obstacles at their pleasure, as well as designate which tools word with which obstacles. The text file is read during map generation, and items are scattered randomly throughout.
3. Map saving and loading was very tricky as well. Custom map creation is accomplished in a similar way as custom item creation--via a text file that is then read by the program.


Other major components included:
- The viewport screen. Implementing this was challenging at first, as if the cursor went out of bounds or the UI did not fit into the alloted right hand corner, the game would break.
- Character pathing. **TODO:** Add detail here?
- Item functionality. This included creating a sort of tool belt that could be rotated through so a player could select one that matches the type of a given obstacle.
- Clue functionality. This was tricky because it required getting involved in so many different areas. It had to have custom UI components to be able to handle the large amount of text, and it needed to be able to dig into the map itself to look for ways to generate the clues (and then sometimes make them incorrect on purpose).
- **TODO**: Any more detail people want to go into...

Lastly, as described in the instructions and shown in our trailer, our entire 
game is working without any errors which means were able to implement all the features according to the requirements of the game.

- GitHub Repo Link:
    
    https://github.com/MaxCarlson/Frupal

- Final presentation video:
    
    https://media.pdx.edu/media/1_plxxixxl
