#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILEMAPINFO_H
    #define TILEMAPINFO_H
    #include "Vec2.h"
    #include "MovingTile.h"

    class TileMapInfo{
        public:
            TileMapInfo(std::string file);
            void Open(std::string file);
            std::map<int,Vec2> portals;     //Keeps where the player will be teleported
            std::map<int,std::vector<std::string>> portalfiles; //Keeps the files of the new map
            std::vector<std::weak_ptr<GameObject>> movingtiles; //Keeps all the movingtiles in the map
    };

#endif