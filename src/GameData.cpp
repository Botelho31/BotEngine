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
    std::cout << std::endl;
    if(Player::player){
        savePlayerPos = Player::player->GetPosition();
        savePlayerHealth = Player::player->GetLife();
        std::cout << "Player Data Saved" << std::endl;
    }else{
        savePlayerHealth = 150;
        std::cout << "Couldn't Find Player Data" << std::endl;
    }
    std::cout << std::endl;
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
                Vec2 loadplayerpos; 
                int playerlife;      
                while(checkline != "map"){
                    FileReader >> checkline;
                }   
                FileReader >> checkpointMap;
                FileReader >> checkline;
                FileReader >> checkpointMapInfo;
                FileReader >> checkline;
                FileReader >> loadplayerpos.x;
                FileReader >> loadplayerpos.y;
                FileReader >> checkline;
                FileReader >> playerlife;

                if(loadplayerpos != Vec2(0,0)){
                    if(Player::player){
                        Player::player->MovePlayer(loadplayerpos.x,loadplayerpos.y,false);
                    }
                }
                
            }
        }
    }else{
        std::cout << "No Save File Found" << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}
