#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef GAMEDATA_H
    #define GAMEDATA_H
    #include "Vec2.h"

    class GameData{
        public:
            static bool playerAlive;
            static std::string checkpointMap;
            static std::string checkpointMapInfo;
            static Vec2 checkpointPos;
    };

#endif