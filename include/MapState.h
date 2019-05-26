#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MAPSTATE_H
    #define MAPSTATE_H
    #include "Timer.h"
    #include "WindowEffects.h"

    class MapState : public State{
        public:
            class Portal{
                public:
                    Portal(){}
                    Vec2 PortalPos;
                    Vec2 PortalPosTo;
                    std::string mapInfoTo;
            };
            class Map{
                public:
                    Map(){}
                    Rect GetMapRect(){
                        return Rect(0,0,width*10,height*10);
                    }
                    std::string mapFile;
                    std::string mapInfoFile;
                    int width,height;
                    int collisionDepthOffset;
                    std::vector<Portal*> portals;
            };
            MapState();
            ~MapState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void GetMapsInfo(std::string maplistfile);
            void GetMapSize(MapState::Map *map);
            
            void Start();
            void Resume();
            void Pause();
        private:
            WindowEffects *windoweffects;

            std::vector<Map*> maps;
    };

#endif