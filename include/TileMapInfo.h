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
    };

#endif