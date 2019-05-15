#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef GAMEDATA_H
    #define GAMEDATA_H
    #include "Vec2.h"

    class GameData{
        public:
            static void SaveGame();
            static void LoadGame();
            static void PrintGameData();

            static bool playerAlive;
            static Vec2 savePlayerPos;
            static int savePlayerHealth;
            static std::string checkpointMap;
            static std::string checkpointMapInfo;
            static Vec2 checkpointPos;
    };

#endif