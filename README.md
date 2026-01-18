# Ti-84 Plus CE Implementation of the crisp-game-lib-portable library

Minimal C-lang library for creating classic arcade-like mini-games running on the Ti-84 Plus CE. Re-implemented version of [crisp-game-lib-portable](https://github.com/abagames/crisp-game-lib-portable) for Ti-84 Plus CE. You can play [sample games in your browser](https://abagames.github.io/crisp-game-lib-portable/build/).

## Sample game codes and reference

- [Sample game codes](https://github.com/chain-linc/ti84pce-crisp-game-lib-portable/tree/main/src/games)
- Reference
  - [Functions and variables](https://abagames.github.io/crisp-game-lib-portable/ref_document/html/cglp_8c.html)
  - [Structs and macros](https://abagames.github.io/crisp-game-lib-portable/ref_document/html/cglp_8h.html)
  - [2d vector functions](https://abagames.github.io/crisp-game-lib-portable/ref_document/html/vector_8c.html) ([macros](https://abagames.github.io/crisp-game-lib-portable/ref_document/html/vector_8h.html))

## How to make a game

1. Copy [game_Template.c](https://raw.githubusercontent.com/chain-linc/ti84pce-crisp-game-lib-portable/main/src/games/game_Template.c) to `game[your game name].c`

1. Add your game in [menuGameList.c](https://github.com/chain-linc/ti84pce-crisp-game-lib-portable/blob/main/src/lib/menuGameList.c) and add `void addGame[your game name]();` and `addGame[your game name]()`

   ```
   ...(snip)...
   void addGameReflector();
   void addGame[your game name]();

   void addGames() {
     /*addGameThunder();
     ...(snip)...
     addGameReflector();*/
     addGame[your game name]();
   }
   ```

1. Write your own game in `game[your game name].c` and rename `void addGame_Template() {` to `void addGame[your game name]() {`

1. build using the [ti-84 Plus CE toolchain](https://ce-programming.github.io/toolchain/) and debug

1. After your done debugging you can send it to your Ti-84 Plus CE

## How to operate
- A - 2nd key
- B - Alpha key
- left/right/up/down - arrow keys
- Exit game to the [game selection menu] - Clear Key
- Exit [game selection menu] to Ti-OS - Clear Key

## Notes
- speed is a issue most game run at 1/3 speed (most likly due to cglp collision detection overhead)
- The sound functions are currently just stubs and sound timer will always return 0
