
#ifndef SDL
  #define SDL
  #define INCLUDE_SDL
  #define INCLUDE_SDL_IMAGE
  #define INCLUDE_SDL_MIXER
  #define INCLUDE_SDL_TTF
  #include "SDL_include.h"
#endif

//VALGRIND COMMAND: valgrind --leak-check=full --track-origins=yes --log-file=valgrindlog.txt ./GAME

#ifndef GAME_H
  #define GAME_H
  // #define DEBUG
  #define PI 3.14159265
  #define ToPI(x) (x * PI)/180
  #define ENDLINE std::cout << std::endl;
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
  #include "Music.h"

  #define PLAYERHP 70
  #define PLAYERMANA 100
  #define PLAYERMANAPERHIT 5
  #define PLAYERMANAPERPROJECTILE 50

  #define PLAYERSWORDDAMAGE 20
  #define PLAYERPROJECTILEDAMAGE 50
  #define MINIONHP 60
  #define MINIONDAMAGE 10

  #define BOSSHP 1000

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

      Music* GetMusic();

    private:
      
      void CalculateDeltaTime();
      float dt;
      int frameStart;

      static Game *instance;
      SDL_Window *window;
      SDL_Renderer *renderer;
      State *storedstate;
      Resources *resources;

      Music *backgroundMusic;

      std::stack<std::unique_ptr<State>> stateStack;
  };

#endif