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
                    Rect GetMapRect(Vec2 scale){
                        return Rect(0,0,width*scale.x,height*scale.y);
                    }
                    int At(int x,int y){
                        if((x < width) && (y < height)){
                            int tilePlace = (y  * width) + x;
                            return tileMatrix[tilePlace];
                        }else{
                            int error = -1;
                            return error;
                        }
                    }
                    int r,g,b;
                    bool printed;
                    std::vector<int> tileMatrix;
                    std::string mapFile;
                    std::string mapInfoFile;
                    int width,height,depth;
                    int collisionDepthOffset;
                    std::vector<Rect> fakewalls;
                    std::vector<Portal*> portals;
            };
            MapState();
            ~MapState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void PrintMap(Map *map,Vec2 pos); // Prints the WholeMap
            void PrintTileMap(Map *map,Vec2 pos); //Prints a single TileMap at specified location
            void GetMapsInfo();  //Gets the map files
            void GetMapSize(Map *map);  //Gets the map matrix and size

            //Aproximate portaloc to side of map for printing
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

            Vec2 sizeOfTile;
            Vec2 printSize;
    };

#endif