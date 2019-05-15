#include "../include/GameData.h"
#include "../include/Player.h"

bool GameData::playerAlive;
Vec2 GameData::savePlayerPos;
int GameData::savePlayerHealth;
std::string GameData::checkpointMap;
std::string GameData::checkpointMapInfo;
Vec2 GameData::checkpointPos;

void GameData::SaveGame(){
    std::ofstream savefile;
    ENDLINE
    if(Player::player){
        savePlayerPos = Player::player->GetPosition();
        savePlayerHealth = Player::player->GetLife();
        std::cout << "Player Data Saved" << std::endl;
    }else{
        savePlayerHealth = 150;
        std::cout << "Couldn't Find Player Data" << std::endl;
    }
    ENDLINE
    savefile.open ("assets/saves/save.txt");
    savefile << "Save\n";
    savefile << "\t[\n";
    savefile << "\t\t" << "map " << checkpointMap << "\n";
    savefile << "\t\t" << "mapInfo " << checkpointMapInfo << "\n";
    savefile << "\t\t" << "playerPos " << savePlayerPos.x << " " << savePlayerPos.y << "\n";
    savefile << "\t\t" << "playerLife " << savePlayerHealth << "\n";
    savefile << "\t]\n";
    savefile.close();
    std::cout << "Game Saved" << std::endl;
}

void GameData::LoadGame(){
    std::fstream FileReader;
    FileReader.open("assets/saves/save.txt");
    std::string checkline;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> checkline;    
            if(checkline == "Save"){  
                while(checkline != "map"){
                    FileReader >> checkline;
                }   
                FileReader >> checkpointMap;
                FileReader >> checkline;
                FileReader >> checkpointMapInfo;
                FileReader >> checkline;
                FileReader >> savePlayerPos.x;
                FileReader >> savePlayerPos.y;
                FileReader >> checkline;
                FileReader >> savePlayerHealth;
                std::cout << "Game Loaded" << std::endl;

            }
        }
    }else{
        std::cout << "No Save File Found" << std::endl; //Printa um erro caso nao consiga dar load na file
        checkpointMap = "assets/map/tileMaptest-1.txt";
        checkpointMapInfo = "assets/map/info/tileMaptest-1.txt";
        checkpointPos = Vec2(100,500);
        savePlayerPos = checkpointPos;
    }
    playerAlive = true;
    FileReader.close();
}

void GameData::PrintGameData(){
    std::cout << "Map: " << checkpointMap << std::endl;
    std::cout << "MapInfo: " << checkpointMapInfo << std::endl;
    std::cout << "PlayerPos: " << savePlayerPos.x << " " << savePlayerPos.y << std::endl;
    std::cout << "PlayerLife: " << savePlayerHealth << std::endl;
}
