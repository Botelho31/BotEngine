
#ifndef SDL
  #define SDL
  #define INCLUDE_SDL
  #define INCLUDE_SDL_IMAGE
  #define INCLUDE_SDL_MIXER
  #include "SDL_include.h"
#endif

#ifndef GAME_H
 #define GAME_H
 #include <iostream>
 #include <vector>
 #include "State.h"
 class Game {

  public:
    ~Game();

    void Run();
    SDL_Renderer* GetRenderer();
    State& GetState();
    static Game& GetInstance();

  private:
    Game(const char* Title,int Width,int Height);
    static Game *instance;
    SDL_Window *window;
    SDL_Renderer *renderer;
    State *state;

};

#endif