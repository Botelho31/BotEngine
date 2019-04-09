#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILEMAPINFO_H
    #define TILEMAPINFO_H
    #include "Vec2.h"

    class TileMapInfo{
        public:
            TileMapInfo(std::string file);
            void Open(std::string file);
            std::map<int,Vec2> portals;
            std::map<int,std::vector<std::string>> portalfiles;
    };

#endif