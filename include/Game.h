
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
  #include <bits/stdc++.h> 
  #include <ctime>
  #include "math.h"
  #include "State.h"
  #include "Resources.h"
  #include "InputManager.h"

  class Game {

    public:
      Game(std::string Title,int Width,int Height);
      ~Game();

      void Run();
      SDL_Renderer* GetRenderer();
      State& GetCurrentState();

      void Push(State *state);

      static Game& GetInstance();

      float GetDeltaTime();

    private:
      
      void CalculateDeltaTime();
      float dt;
      int frameStart;

      static Game *instance;
      SDL_Window *window;
      SDL_Renderer *renderer;
      State *storedstate;
      Resources *resources;

      std::stack<std::unique_ptr<State>> stateStack;
  };

#endif