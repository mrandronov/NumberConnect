# CTicTacToe-UI

- [Explanation](#explanation)
    - [Game Logic](#game-logic)
    - [GUI Implementation](#gui-implementation)
- [Building](#building)
- [Execution](#execution)
    - [How to Play](#how-to-play)
    - [Themes](#themes)
    - [Future Ideas](#future-ideas)

Tic-Tac-Toe game with a friendly UI implemented in C.

The UI is written entirely using SDL2. The `CMakeLists.txt` has mention of OpenGL ( although it should be removed ), however OpenGL is not used at all in this project.

## Explanation

### Game Logic

The game logic itself is a relatively short amount of code. This is mainly implemented using a struct for the game state and structs for a human and computer player respectively. The game logic is a bit messy, in that some design decision were made to have the game be extensible ( i.e, two 'human' players can play the game, player markers can be customized, etc., ).

The computer AI is mini-max algorithm, with an attempt at Alpha-beta pruning included. I need to review the logic for this, because it still does not make efficient choices. That said, it works well enough in its current state to be playable.

### GUI Implementation

The ui is designed pretty similarly to a *Model-View-Controller* type architecture, but it is not at all discrete.

There are 4 UI component types:
- Buttons
- Selectors
- Labels
- Board Display ( The tic-tac-toe game board, comprised of a mixture of selectors and labels )

Each component is implemented as separate structs and have their own functions for drawing and any action logic that they may have. User input to buttons and selectors are made using a kind of 'listener', in that a button struct will have a function that gets called whenever a particular event is received from SDL2's input event management queues. 
 
When an event is received from SDL2's library, lists for selectors, buttons, labels, or whatever else I implemented, are iterated through and if a condition is satisfied for that component ( i.e, the mouse was over it ), then the appropriate function pointed to by the struct is called. This process occurs for every iteration of the main game loop which should be happening every frame.

The `registration` of a listener is done at compile time, meaning this stuff is essentially hardcoded. In the future however, I would like to implement a more graceful and elegant `pub-sub` design for components. It would, idealy, be more extensible, more efficient, and make building new components simpler. On the horizon, I'm thinking of writing a custom UI library for all of this.

## Building

Make sure you have SDL2 and SDL2_ttf libraries installed on your system. They are **not** included in this project, however that is a future improvement for the repo.

To build, run:
```
mkdir build && cd build
cmake ..
make
```

## Execution

To run the game, simply do:
```
./build/TicTacToeGame
```

### How to Play

The game works by simply clicking on the squares to place a move and the computer will immediately follow up with a turn of its own. The AI is not perfect and is a bit of a work in progress, but if you don't pay attention it might beat you! When a game is over, press `NEW GAME` to start over.

You can switch your marker between 'O' and 'X', but only in between new games. If you switch the setting, you have to press `NEW GAME` to see your changes come into effect.

When you are done playig, press the `QUIT` button underneath the `NEW GAME` button or the close button at the top to quit the game.

### Themes

This game also implements `Themes`. There is a directory filled with `.th` files which store colors that are used in the game. You can specify a different theme of the game when launch by passing the path to the theme file as an argument to the game executable. 

For example, the following will load the `Cool Blue` theme:
```
./build/TicTacToeGame ./theme/Cool_Blue.th
```

A few themes to try: `Cool_Blue.th`, `Purple_Scotch.th`, `Lateral_Rose.th`, `Warm_Office.th`, and `Vita_Anima.th`.

The default theme is used when no arguments are passed.

## Future Ideas
- [ ] Build a UI library using a View Tree and write a UI application using the MVC design pattern.
- [ ] Allow custom player markers.
- [ ] Improve the computer AI to be undefeatable.
- [ ] Allow Human-Human mode.
