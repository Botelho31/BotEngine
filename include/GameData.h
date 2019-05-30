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

            static std::map<std::string,std::string> GetSpritesFiles(std::string spritesfile);

            static std::string ParseTMX(std::string filetmx);   //Parses TMX and returns TXT tilemap file
            static std::vector<int> ParseTileMap(std::string tilemapfile,int& width,int& height,int& depth); //Parses TXT tilemap and returns tilematrix
            static std::string GetExtension(std::string file);
            static std::string SetExtension(std::string file,std::string ext);
            static std::string AddToPath(std::string file,std::string newpath);
            static Vec2 GetSizeOfPng(std::string pngfile);

            static bool playerAlive;
            static Vec2 savePlayerPos;
            static int savePlayerHealth;
            static std::string checkpointMapInfo;
            static Vec2 checkpointPos;
            static Vec2 checkpointPosSpeed;

            static std::queue<Event*> events;
    };

#endif