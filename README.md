# CENG209_HW_19050141022
# Adventure Game

## About the Game
The Adventure Game is a text-based exploration game written in C. It runs in the command line and allows players to explore rooms, collect items, battle enemies, and save or load their progress. It is designed for simplicity and focuses on navigation, combat, and inventory management.

## Key Features
- **Navigate Rooms**: Move between connected rooms using directional commands.
- **Collect Items**: Gather items scattered throughout the rooms to build your inventory.
- **Combat System**: Fight enemies in a turn-based style until one of you is defeated.
- **Save and Load Progress**: Save your current game to a file and load it back later.
- **List Saved Games**: Display a list of saved games available in the current directory.

## How to Play
To play the game, type in commands at the prompt. Here is a list of supported commands:

- **move <direction>**: Move to a different room. Replace <direction> with north, south, east, or west.
  - Example: move north

- **look**: See the current room's description, any items in the room, and if there are enemies present.

- **pickup <item>**: Pick up an item from the room. Replace <item> with the name of the item.
  - Example: pickup key

- **inventory**: Show the items you currently have in your inventory.

- **save <filename>**: Save the current game state to a file.
  - Example: save mygame.txt

- **load <filename>**: Load a saved game from a file.
  - Example: load mygame.txt

- **list**: Show all available saved game files in the current directory.

- **exit**: Quit the game.

## How to Compile and Run the Game
### Compiling
To compile the game, use the provided Makefile:

make

This will compile the game and create an executable file named adventure_game.

### Running
After compilation, you can run the game with this command:
./adventure_game

### Cleaning Up
To clean up the compiled files, use:
make clean

## Example Session
Here is an example of what gameplay might look like:
```text
Welcome to the Adventure Game!
> look
You are in a dark cave. It feels cold and damp.
Items available:
- torch

> pickup torch
You picked up torch.

> move east
You head east.
You are now in a sunny clearing.

> save game1.txt
Game saved at game1.txt.

> exit
Thanks for playing the Adventure Game!
```

