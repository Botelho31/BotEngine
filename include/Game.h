
#ifndef SDL
  #define SDL
  #define INCLUDE_SDL
  #define INCLUDE_SDL_IMAGE
  #define INCLUDE_SDL_MIXER
  #include "SDL_include.h"
#endif

#ifndef GAME_H
  #define GAME_H
  #define PI 3.14159265
  #include <iostream>
  #include <fstream>
  #include <vector>
  #include <memory>
  #include <cstdlib>
  #include<bits/stdc++.h> 
  #include <ctime>
  #include "math.h"
  #include "State.h"
  #include "Resources.h"

  class Game {

    public:
      ~Game();

      void Run();
      SDL_Renderer* GetRenderer();
      State& GetState();
      static Game& GetInstance();

    private:
      Game(std::string Title,int Width,int Height);
      static Game *instance;
      SDL_Window *window;
      SDL_Renderer *renderer;
      State *state;
      Resources *resources;

  };

#endif