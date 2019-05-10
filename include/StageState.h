#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef STAGESTATE_H
    #define STAGESTATE_H
    #include "Music.h"
    #include "State.h"
    #include "TileSet.h"
    #include "TileMap.h"
    #include "Text.h"
    #include "WindowEffects.h"

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

            void HandleTileEvents(Vec2 PlayerPos);
            void ClearMobs();
            void UpdateHP();
            
            static bool ChangingMap();  //Check if the tilemap is changing
        private:
            TileSet *tileset;
            Music *backgroundMusic;
            Text *playerhp;
            WindowEffects *windoweffects;

            bool mapcollision;
            static bool changingMap;    //Keeps the bool if the map is changing
            int nextMap;    //Keeps the value of the portal that references the new map
            Vec2 playerspeed;   //Keeps the player speed pre map exchange
            Vec2 playerpos;     //Keeps the player pos when he changes map
    };

#endif