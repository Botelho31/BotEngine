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
                    Rect PortalBox;
                    Vec2 PortalPosTo;
                    std::string mapInfoTo;
            };
            class Map{
                public:
                    Map(){}
                    Rect GetMapRect(){
                        return Rect(0,0,width*10,height*10);
                    }
                    int r,g,b;
                    bool printed;
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

            void PrintMap(Map *map,Vec2 pos);
            void GetMapsInfo(std::string maplistfile);
            void GetMapSize(Map *map);

            Vec2 ApproximateToSideOfMap(Map *map,Rect pos);
            Vec2 ApproximateToSideOfMap(Map *map,Vec2 pos);
            
            void Start();
            void Resume();
            void Pause();
        private:
            WindowEffects *windoweffects;

            bool centeronplayer;
            GameObject *playerIcon;
            std::vector<Map*> maps;
    };

#endif