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

            void ExpandTileColliders(); //Handles TileColliders expansion in the opening of new TileMaps
            void HandleEvents(); //Handles event
            void ClearMobs(); //Clear all mobs and hitboxes from screen
            void UpdateHP();  //Updates the playerHP being displayed on HUD
            
            static bool ChangingMap();  //Check if the tilemap is changing
        private:
            TileSet *tileset;
            Music *backgroundMusic;
            Text *playerhp;
            bool showfps;
            Text *fps;
            WindowEffects *windoweffects;

            bool mapcollision; //Boolean indicating if mapcollision is ready or not
            static bool changingMap;    //Keeps the bool if the map is changing
            int nextMap;    //Keeps the value of the portal that references the new map
    };

#endif