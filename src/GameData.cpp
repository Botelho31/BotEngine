#include "../include/GameData.h"
#include "../include/Player.h"
#include "../include/Resources.h"

std::vector<std::string> GameData::saveFiles;
bool GameData::playerAlive;
bool GameData::playerSword;

Vec2 GameData::savePlayerPos;
int GameData::savePlayerHealth;
int GameData::savePlayerMana;
std::string GameData::checkpointMapInfo;
Vec2 GameData::checkpointPos;
Vec2 GameData::checkpointPosSpeed;
std::vector<std::string> GameData::listOfDiscoveredMaps;
std::vector<int> GameData::listOfDiscoveredSouls;

std::string GameData::currentMusic;
std::queue<Event*> GameData::events;


bool GameData::ChooseSave(int chosenI){
    if((chosenI >= 0) && (chosenI < saveFiles.size())){
        if(chosenI == 0){
            return true;
        }else{
            std::iter_swap(saveFiles.begin(),saveFiles.begin() + chosenI);
        }
    }else{
        std::cout << "Chosen Save does not Exist" << std::endl;
        return false;
    }
}

void GameData::GetListOfSaves(std::string savelist){
    std::fstream FileReader;
    FileReader.open(savelist);
    std::string checkline;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> checkline;    
            if(checkline == "ListOfSaves"){  
                FileReader >> checkline;
                while(checkline != "]"){
                    if((checkline != "]") && (checkline != "[")){
                        saveFiles.push_back(checkline);
                    }
                    FileReader >> checkline;
                }
            }
        }
    }else{
        ENDLINE
        std::cout << "No Save on Save List" << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}

void GameData::SaveListOfSaves(std::string savelist){
    std::ofstream savefile;
    savefile.open (savelist);
    savefile << "ListOfSaves\n";
    savefile << "\t[\n";
    for(int i = 0;i < saveFiles.size();i++){
        savefile << "\t\t" << saveFiles[i] << "\n";
    }
    savefile << "\t]\n";
    savefile.close();
}



bool GameData::AddSave(std::string savename){
    std::string savefile = AddToPath(AddToPath(SetExtension(savename,"txt"),"assets"),"saves");
    std::cout << savefile << std::endl;
    bool saveExists = false;
    for(int i = 0;i < saveFiles.size();i++){
        if(saveFiles[i] == savefile){
            saveExists = true;
        }
    }
    if(!saveExists){
        saveFiles.push_back(savefile);
        return true;
    }else{
        std::cout << "Save Already Exists" << std::endl;
        return false;
    }
}

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
        savePlayerHealth = PLAYERHP;
        std::cout << "Couldn't Find Player Data" << std::endl;
    }
    ENDLINE
    savefile.open (saveFiles[0]);
    savefile << "Save\n";
    savefile << "\t[\n";
    savefile << "\t\t" << "mapInfo " << checkpointMapInfo << "\n";
    savefile << "\t\t" << "playerPos " << savePlayerPos.x << " " << savePlayerPos.y << "\n";
    savefile << "\t\t" << "playerLife " << savePlayerHealth << "\n";
    savefile << "\t\t" << "playerMana " << savePlayerMana << "\n";
    savefile << "\t\t" << "checkpointPos " << checkpointPos.x << " " << checkpointPos.y << "\n";
    savefile << "\t\t" << "checkpointPosSpeed " << checkpointPosSpeed.x << " " << checkpointPosSpeed.y << "\n";
    savefile << "\t]\n";
    savefile << "ListOfDiscoveredMaps\n";
    savefile << "\t[\n";
    for(int i = 0;i < listOfDiscoveredMaps.size();i++){
        savefile << "\t\t" << listOfDiscoveredMaps[i] << "\n";
    }
    savefile << "\t]\n";
    savefile << "ListOfDiscoveredSouls\n";
    savefile << "\t[\n";
    for(int i = 0;i < listOfDiscoveredSouls.size();i++){
        savefile << "\t\t" << listOfDiscoveredSouls[i] << "\n";
    }
    savefile << "\t]\n";
    savefile.close();
    std::cout << "Game Saved" << std::endl;
}

void GameData::LoadGame(){
    std::fstream FileReader;
    FileReader.open(saveFiles[0]);
    std::string checkline;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> checkline;    
            if(checkline == "Save"){  
                while(checkline != "mapInfo"){
                    FileReader >> checkline;
                }
                FileReader >> checkpointMapInfo;
                FileReader >> checkline;
                FileReader >> savePlayerPos.x;
                FileReader >> savePlayerPos.y;
                FileReader >> checkline;
                FileReader >> savePlayerHealth;
                FileReader >> checkline;
                FileReader >> savePlayerMana;
                FileReader >> checkline;
                FileReader >> checkpointPos.x;
                FileReader >> checkpointPos.y;
                FileReader >> checkline;
                FileReader >> checkpointPosSpeed.x;
                FileReader >> checkpointPosSpeed.y;
                ENDLINE
                std::cout << "Game Loaded" << std::endl;
            }
            if(checkline == "ListOfDiscoveredMaps"){
                FileReader >> checkline;
                while(checkline != "]"){
                    if((checkline != "]") && (checkline != "[")){
                        AddMap(checkline);
                    }
                    FileReader >> checkline;
                }
            }
            if(checkline ==  "ListOfDiscoveredSouls"){
                FileReader >> checkline;
                while(checkline != "]"){
                    if((checkline != "]") && (checkline != "[")){
                        std::stringstream soul;
                        int soulID;
                        soul << checkline;
                        soul >> soulID;
                        listOfDiscoveredSouls.push_back(soulID);
                    }
                    FileReader >> checkline;
                }
                if(listOfDiscoveredSouls.size() >= 3){
                    playerSword = true;
                }
            }
        }
    }else{
        ENDLINE
        std::cout << "No Save File Found" << std::endl; //Printa um erro caso nao consiga dar load na file
        checkpointMapInfo = "assets/map/info/EntryRoom.txt";
        checkpointPos = Vec2(3120,1530);
        savePlayerHealth = PLAYERHP;
        savePlayerMana = PLAYERMANA;
        savePlayerPos = checkpointPos;
        AddMap("assets/map/info/EntryRoom.txt");
    }
    playerAlive = true;
    FileReader.close();
}

void GameData::AddMap(std::string map){
    bool discovered = false;
    for(int i = 0;i < listOfDiscoveredMaps.size();i++){
        if(map == listOfDiscoveredMaps[i]){
            discovered = true;
        }
    }
    if(!discovered){
        listOfDiscoveredMaps.push_back(map);
    }
}

void GameData::PrintGameData(){
    ENDLINE
    std::cout << "MapInfo: " << checkpointMapInfo << std::endl;
    std::cout << "PlayerPos: " << savePlayerPos.x << " " << savePlayerPos.y << std::endl;
    std::cout << "PlayerLife: " << savePlayerHealth << std::endl;
}

void GameData::PreLoadSprites(std::string spritefile){
    std::vector<std::string> spritefilevec;
    spritefilevec.push_back(spritefile);
    std::map<std::string,std::string> spritefiles = GetSpritesFiles(spritefilevec);
     for(auto it = spritefiles.cbegin(); it != spritefiles.cend(); ++it){
        Resources::GetImage(it->second);
    }
}

void GameData::PreLoadSounds(std::string spritefile){
    std::vector<std::string> spritefilevec;
    spritefilevec.push_back(spritefile);
    std::map<std::string,std::string> spritefiles = GetSpritesFiles(spritefilevec);
     for(auto it = spritefiles.cbegin(); it != spritefiles.cend(); ++it){
        Resources::GetSound(it->second);
    }
}

std::map<std::string,std::string> GameData::GetSpritesFiles(std::vector<std::string> spritesfile){
    std::map<std::string,std::string> filenames;

    for(int i = 0;i < spritesfile.size();i++){
        std::fstream FileReader;
        FileReader.open(spritesfile[i]);
        std::string checkline;
        if (FileReader.is_open()) {
            while (!FileReader.eof()) {
                FileReader >> checkline;    
                if(checkline == "["){  
                    while(checkline != "]"){
                        FileReader >> checkline; 
                        if(checkline != "]"){
                            std::string name = checkline;
                            FileReader >> checkline; 
                            std::string file = checkline;
                            filenames.insert({name,file});
                        }
                    }
                }
            }
        }else{
            ENDLINE
            std::cout << "No Sprite File Found: " << spritesfile[i] << std::endl; //Printa um erro caso nao consiga dar load na file
        }
        FileReader.close();
    }
    return filenames;
}

std::string GameData::ParseTMX(std::string filetmx){

    std::string newfilename = SetExtension(filetmx,"txt");
   newfilename = AddToPath(newfilename,"txts");
    std::ifstream FileReader;
    FileReader.open(filetmx);
    std::string checkline;
    if (FileReader.is_open()) {
        int width = 0;
        int height = 0;
        int depth = 0;
        bool valuesget = false;
        std::stringstream numberstring;
        std::stringstream mapcontent;
        std::ofstream txtfile;
        txtfile.open (newfilename);
        while (!FileReader.eof()) {
            FileReader >> checkline;
            if((checkline == "layer") && (!valuesget)){
                std::getline(FileReader, checkline, '"');
                std::getline(FileReader, checkline, '"');
                depth ++;

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
                std::getline(FileReader, checkline, '<');

                valuesget = true;
            }else if(checkline == "layer"){
                std::getline(FileReader, checkline, '<');
                depth ++;
            }
            else if(checkline == "data"){
                std::getline(FileReader, checkline, '>');
                std::getline(FileReader, checkline, '<');
                mapcontent << checkline << ",";
            }else{
                std::getline(FileReader, checkline, '<');
            }
        }
        std::stringstream finalvalues;
        finalvalues << width << "," << height << "," << depth << "," << mapcontent.str();
        txtfile << finalvalues.str();
        txtfile.close();
    }else{
        ENDLINE
        std::cout << "Couldnt open tilemap TMX: " << filetmx << std::endl;
        newfilename = "";
    }
    FileReader.close();

    return newfilename;
}  

std::vector<int> GameData::ParseTileMap(std::string tilemapfile,int& width,int& height,int& depth){
    if(GameData::GetExtension(tilemapfile) == "tmx"){
        tilemapfile = GameData::ParseTMX(tilemapfile);
    }
    std::ifstream FileReader;
    FileReader.open(tilemapfile);
    std::stringstream num;
    std::string number;
    std::vector<int> tileMatrix;
    int numint = 0;
    int iterator = 0;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            num.clear();
            std::getline(FileReader, number, ',');
            num << number;
            num >> numint;
            if(iterator == 0){
                width = numint;
            }
            else if(iterator == 1){
                height = numint;
            }
            else if(iterator == 2){
                depth = numint;
            }
            else{
                tileMatrix.push_back(numint-1);
            }
            iterator ++;
        }
    }else{
        std::cout << "Couldnt open tilemap: " << tilemapfile << std::endl;
    }
    FileReader.close();

    return tileMatrix;
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

std::string GameData::AddToPath(std::string file,std::string newpath){
    std::string checkline;
    std::stringstream newfilestream;
    std::stringstream filestream;
    std::vector<std::string> path;
    filestream << file;
    while(!filestream.eof()){
        std::getline(filestream,checkline,'/');
        path.push_back(checkline);
    }

    for(int i = 0;i < path.size() - 1;i++){
        newfilestream << path[i] << "/";
    }
    newfilestream << newpath << "/";
    newfilestream << path[path.size() - 1];

    return newfilestream.str();
}

Vec2 GameData::GetSizeOfPng(std::string pngfile){
    Vec2 size;
    FILE *f=fopen(pngfile.c_str(),"rb");
    if (f==0){
        return Vec2(0,0);
    }
    fseek(f,0,SEEK_END);
    long len=ftell(f);
    fseek(f,0,SEEK_SET);
    if (len<24) {
        fclose(f);
        return Vec2(0,0);
    }
    unsigned char buf[24]; fread(buf,1,24,f);

    size.x = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0);
    size.y = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0);

    return size;
}