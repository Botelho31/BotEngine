#include "../include/GameData.h"
#include "../include/Player.h"

bool GameData::playerAlive;
Vec2 GameData::savePlayerPos;
int GameData::savePlayerHealth;
std::string GameData::checkpointMap;
std::string GameData::checkpointMapInfo;
Vec2 GameData::checkpointPos;
Vec2 GameData::checkpointPosSpeed;

std::queue<Event*> GameData::events;

void GameData::SaveGame(){
    std::ofstream savefile;
    ENDLINE
    if(Player::player){
        savePlayerPos = Player::player->GetPosition();
        if(checkpointPos == Vec2(0,0)){
            checkpointPos = savePlayerPos;
        }
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
    savefile << "\t\t" << "checkpointPos " << checkpointPos.x << " " << checkpointPos.y << "\n";
    savefile << "\t\t" << "checkpointPosSpeed " << checkpointPosSpeed.x << " " << checkpointPosSpeed.y << "\n";
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
                FileReader >> checkline;
                FileReader >> checkpointPos.x;
                FileReader >> checkpointPos.y;
                FileReader >> checkline;
                FileReader >> checkpointPosSpeed.x;
                FileReader >> checkpointPosSpeed.y;
                ENDLINE
                std::cout << "Game Loaded" << std::endl;
                ParseTMX("assets/map/tileMaptest-1-FORBACKGROUNDTEST.tmx");
            }
        }
    }else{
        ENDLINE
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
    ENDLINE
    std::cout << "Map: " << checkpointMap << std::endl;
    std::cout << "MapInfo: " << checkpointMapInfo << std::endl;
    std::cout << "PlayerPos: " << savePlayerPos.x << " " << savePlayerPos.y << std::endl;
    std::cout << "PlayerLife: " << savePlayerHealth << std::endl;
}

std::string GameData::ParseTMX(std::string filetmx){

    std::ofstream txtfile;
    std::string newfilename = SetExtension(filetmx,"txt");
    txtfile.open (newfilename);

    int width,height,depth;
    bool valuesget = false;
    std::stringstream numberstring;

    std::ifstream FileReader;
    FileReader.open(filetmx);
    std::string checkline;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> checkline;
            if((checkline == "layer") && (!valuesget)){
                std::getline(FileReader, checkline, '"');
                std::getline(FileReader, checkline, '"');
                numberstring.clear();
                numberstring << checkline;
                numberstring >> depth;

                std::getline(FileReader, checkline, '"');
                std::getline(FileReader, checkline, '"');

                std::getline(FileReader, checkline, '"');
                std::getline(FileReader, checkline, '"');
                numberstring.clear();
                numberstring << checkline;
                numberstring >> width;

                std::getline(FileReader, checkline, '"');
                std::getline(FileReader, checkline, '"');
                numberstring.clear();
                numberstring << checkline;
                numberstring >> height;

                std::stringstream finalvalues;
                finalvalues << width << "," << height << "," << depth << "," << std::endl;
                txtfile << finalvalues.str();
                valuesget = true;
            }
            if(checkline == "data"){
                std::getline(FileReader, checkline, '>');
                std::getline(FileReader, checkline, '<');
                txtfile << checkline << ",";
            }else{
                std::getline(FileReader, checkline, '<');
            }
        }
    }else{
        ENDLINE
        std::cout << "Couldnt open tilemap TMX: " << filetmx << std::endl;
    }
    txtfile.close();
    FileReader.close();

    return newfilename;
}   

std::string GameData::GetExtension(std::string file){
    std::stringstream filestream;
    std::string checkfile;
    std::string ext;
    filestream << file;
    std::getline(filestream,checkfile,'.');
    filestream >> ext;

    return ext;
}

std::string GameData::SetExtension(std::string file,std::string ext){
    std::stringstream filestream;
    std::string checkfile;
    filestream << file;
    std::getline(filestream,checkfile,'.');
    std::stringstream newfilestream;
    newfilestream << checkfile;
    newfilestream << "." << ext;

    return newfilestream.str();
}
