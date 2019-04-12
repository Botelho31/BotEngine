#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef STAGESTATE_H
    #define STAGESTATE_H
    #include "Music.h"
    #include "State.h"
    #include "TileSet.h"
    #include "TileMap.h"

    class StageState : public State{
        public:
            StageState();
            ~StageState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void Start();
            void Resume();
            void Pause();

            void ClearMobs();
            
            bool QuitRequested();
            static bool ChangingMap();  //Check if the tilemap is changing
        private:
            TileSet *tileset;
            Music *backgroundMusic;
            static bool changingMap;
            int nextMap;    //Keeps the value of the portal that references the new map
    };

#endif