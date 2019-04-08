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

            bool QuitRequested();

        private:
            int currentMap;
            TileSet *tileset;
            Music *backgroundMusic;
    };

#endif