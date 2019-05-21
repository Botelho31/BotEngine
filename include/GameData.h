#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef GAMEDATA_H
    #define GAMEDATA_H
    #include "Vec2.h"
    #include "Event.h"

    class GameData{
        public:
            static void SaveGame();
            static void LoadGame();
            static void PrintGameData();

            static std::string ParseTMX(std::string filetmx);
            static std::string GetExtension(std::string file);
            static std::string SetExtension(std::string file,std::string ext);

            static bool playerAlive;
            static Vec2 savePlayerPos;
            static int savePlayerHealth;
            static std::string checkpointMap;
            static std::string checkpointMapInfo;
            static int checkpointCollisionDepth;
            static Vec2 checkpointPos;
            static Vec2 checkpointPosSpeed;

            static std::queue<Event*> events;
    };

#endif