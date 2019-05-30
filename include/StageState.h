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
    #include "Light.h"

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
            void HandleEvents(float dt); //Handles event
            void ClearMobs(); //Clear all mobs and hitboxes from screen
            void UpdateHP();  //Updates the playerHP being displayed on HUD
            
            static bool ChangingMap();  //Check if the tilemap is changing
            static bool MapCollisionLoaded();
            static bool LoadedTileColliders();
        private:
            TileSet *tileset;
            Music *backgroundMusic;
            bool showfps;
            Text *fps;
            WindowEffects *windoweffects;

            Light *light;

            //HUD
            int playerHP;
            std::vector<std::weak_ptr<GameObject>> playerHPIcons;


            Timer *changingMapTimer; //Timer to allow player to move freely after entering new map
            bool pause;
            int initialtiles;
            static bool loadedTileColliders;
            static bool mapcollision; //Boolean indicating if mapcollision is ready or not
            static bool changingMap;    //Keeps the bool if the map is changing
    };

#endif