#include "../include/StageState.h"
#include "../include/Sprite.h"
#include "../include/Vec2.h"
#include "../include/Sound.h"
#include "../include/TileSet.h"
#include "../include/TileMap.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/CameraFollower.h"
#include "../include/Collision.h"
#include "../include/Collider.h"
#include "../include/GameData.h"
#include "../include/EndState.h"
#include "../include/Player.h"
#include "../include/Minion.h"

bool StageState::changingMap;

StageState::StageState(){
    quitRequested = false;
    popRequested = false;
    started = false;
    backgroundMusic = nullptr;
    changingMap = false;

    backgroundMusic = new Music("assets/audio/stageState.ogg");
    backgroundMusic->Play();

	GameObject *background = new GameObject();
    Sprite *bg = new Sprite(*background,"assets/img/fundobranco.jpg");
    CameraFollower *camerafollower = new CameraFollower(*background);
    bg->SetScaleX(4,4);
	background->AddComponent(bg);
    background->AddComponent(camerafollower);
	objectArray.emplace_back(background);

	GameObject *tileObj = new GameObject();
    GameObject *tilesetObj = new GameObject();
	this->tileset = new TileSet(tilesetObj,32,32,"assets/img/basictiletest.png");
	this->tilemap = new TileMap(*tileObj,"assets/map/tileMaptest-1.txt",tileset);
	tileObj->box.x = 0;
	tileObj->box.y = 0;
	tileObj->AddComponent(tilemap);
	objectArray.emplace_back(tileObj);

    GameObject *playerObj = new GameObject();
    Player *player = new Player(*playerObj);
    playerObj->box.x = 500;
    playerObj->box.y = 100;
    playerObj->AddComponent(player);
    objectArray.emplace_back(playerObj);
    Camera::Follow(playerObj);
}

StageState::~StageState(){
    if(backgroundMusic){
        delete backgroundMusic;
    }
	std::cout << "Cleared "<< objectArray.size() << " Objects" << std::endl;
    objectArray.clear();
}

void StageState::LoadAssets(){

}

void StageState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    Camera::Update(dt);
	if(input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(ESCAPE_KEY)){
        popRequested = true;
    }
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Update(dt);
    }
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component1 = objectArray[i]->GetComponent("Collider");
        if((component1) && ((i + 1) < objectArray.size())){
            Collider *collider1 = dynamic_cast<Collider*>(component1);
            for(unsigned int j = i + 1; j < objectArray.size();j++){
                Component *component2 = objectArray[j]->GetComponent("Collider");
                if(component2){
                    Collider *collider2 = dynamic_cast<Collider*>(component2);
                    if(Collision::IsColliding(collider1->box,collider2->box,(objectArray[i]->angleDeg * PI) /180,(objectArray[j]->angleDeg * PI) /180)){
                        objectArray[i]->NotifyCollision(*objectArray[j]);
                        objectArray[j]->NotifyCollision(*objectArray[i]);
                    }
                }
            }
        }
    }
    for(unsigned int i = 0; i < objectArray.size();i++){
        if(objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin() + i);
        }
    }

    //TILE MAP EXCHANGE
    Vec2 PlayerPos = Vec2(0,0);
    int tilemapID = 0;
    if(Player::player){
        PlayerPos = Player::player->GetPosition();
        tilemapID = tilemap->AtLocation(PlayerPos.x,PlayerPos.y);
    }
    if((tilemapID < -1) || (changingMap)){
        if(!changingMap){
            nextMap = tilemapID + 1;
            changingMap = true; 
        }
        Vec2 PlayerPos = Player::player->GetPosition();
        int tilemapLoc = tilemap->AtLocation(PlayerPos.x,PlayerPos.y); 
        if(tilemapLoc == -1000){
            std::vector<std::string> files = tilemap->GetPortalFiles(nextMap);
            Vec2 portalloc = tilemap->GetPortalLoc(nextMap);
            tilemap->Load(files[0]);
            ClearMobs();
            tilemap->LoadInfo(files[1]);
            Player::player->MovePlayer(portalloc.x,portalloc.y);
            changingMap = false;
        }else if((tilemapLoc != -1000) && (tilemapLoc != (nextMap -1))){
            changingMap = false;
        }
    }
    //END TILEMAP EXCHANGE
}

void StageState::Render(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component = objectArray[i]->GetComponent("TileMap");
        if(component){
            objectArray[i]->box.x = Camera::pos.x;
            objectArray[i]->box.y = Camera::pos.y;
        }
        objectArray[i]->Render();
    }
}

void StageState::ClearMobs(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component = objectArray[i]->GetComponent("Minion");
        if(component){
            objectArray.erase(objectArray.begin() + i);
        }
    }
}

bool StageState::QuitRequested(){
    return quitRequested;
}

bool StageState::ChangingMap(){
    return changingMap;
}

void StageState::Start(){
    StartArray();
    this->tilemap->LoadInfo("assets/map/info/tileMaptest-1.txt");
}

void StageState::Pause(){

}

void StageState::Resume(){

}