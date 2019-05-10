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
#include "../include/TransitionState.h"
#include "../include/MovingTile.h"
#include "../include/TileCollider.h"

bool StageState::changingMap;

StageState::StageState(){
    quitRequested = false;
    popRequested = false;
    started = false;
    backgroundMusic = nullptr;
    changingMap = false;
    windoweffects = new WindowEffects();

    backgroundMusic = new Music("assets/audio/stageState.ogg");
    // backgroundMusic->Play();

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

    GameObject *playerhpObj = new GameObject();
    std::stringstream playerLife;
    playerLife << player->GetLife();
    this->playerhp = new Text(*playerhpObj,"assets/font/Callmemaybe.ttf",100,Text::BLENDED,playerLife.str(),{0,0,0});
    CameraFollower *camerafollower2 = new CameraFollower(*playerhpObj);
    playerhpObj->AddComponent(camerafollower2);
    playerhpObj->AddComponent(playerhp);
    playerhpObj->box.x = 0;
    playerhpObj->box.y = 0;
    objectArray.emplace_back(playerhpObj);

    GameData::playerAlive = true;
    GameData::checkpointMap = "assets/map/tileMaptest-1.txt";
    GameData::checkpointMapInfo = "assets/map/info/tileMaptest-1.txt";
    GameData::checkpointPos =   Vec2(playerObj->box.x,playerObj->box.y);    
}

StageState::~StageState(){
    playerhp = nullptr;
    delete windoweffects;
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
        Component *component1 = objectArray[i]->GetComponent("TileCollider");
        if((component1) && ((i + 1) < objectArray.size())){
            TileCollider *tilecollider1 = dynamic_cast<TileCollider*>(component1);
            for(unsigned int j = i + 1; j < objectArray.size();j++){
                Component *component2 = objectArray[j]->GetComponent("TileCollider");
                if(component2){
                    TileCollider *tilecollider2 = dynamic_cast<TileCollider*>(component2);
                    if(Collision::IsColliding(tilecollider1->box,tilecollider2->box,(objectArray[i]->angleDeg * PI) /180,(objectArray[j]->angleDeg * PI) /180)){
                        objectArray[i]->NotifyCollision(*objectArray[j]);
                        objectArray[j]->NotifyCollision(*objectArray[i]);
                    }
                }
            }
        }
    }
    for(int i = (objectArray.size() - 1); i >= 0;--i){
        if(objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin() + i);
        }
    }
    UpdateHP();
    windoweffects->Update(dt);
    
    if(Player::player){
        Vec2 PlayerPos = Player::player->GetPosition(); 
        HandleTileEvents(PlayerPos);    //HANDLE TILEMAP EXCHANGE
    }

    //HANDLES PLAYER DEATH
    if(!GameData::playerAlive){
        if(Player::player){
            ClearMobs();
            tilemap->Load(GameData::checkpointMap);
            tilemap->LoadInfo(GameData::checkpointMapInfo);
            Player::player->MovePlayer(GameData::checkpointPos.x,GameData::checkpointPos.y,false);
            Player::player->HealPlayer(150);
            GameData::playerAlive = true;
        }
    }
    //END HANDLE PLAYER DEATH
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
    windoweffects->Render();
}

void StageState::ClearMobs(){    
    for(int i = (objectArray.size() - 1); i >= 0; --i){
        Component *component1 = objectArray[i]->GetComponent("HitBox");
        Component *component2 = objectArray[i]->GetComponent("Minion");
        Component *component3 = objectArray[i]->GetComponent("MovingTile");
        Component *component4 = objectArray[i]->GetComponent("TileCollider");
        if(component1){
            objectArray.erase(objectArray.begin() + i);
        }
        if(component2){
            objectArray.erase(objectArray.begin() + i);
        }
        if(component3){
            objectArray.erase(objectArray.begin() + i);
        }
        if(component4){
            objectArray.erase(objectArray.begin() + i);
        }
    }
}

void StageState::HandleTileEvents(Vec2 PlayerPos){
    int tilemapLoc = tilemap->AtLocation(PlayerPos.x,PlayerPos.y);

    //TILEMAP EXCHANGE
    if((tilemapLoc < -1) || (changingMap)){
        if(!changingMap){
            nextMap = tilemapLoc + 1;
            changingMap = true; 
        }
        tilemapLoc = tilemap->AtLocation(PlayerPos.x,PlayerPos.y); 
        
        if(tilemapLoc == -1000){
            if(windoweffects->GetCurrentEffect() == WindowEffects::FADEFROMBLACK){
                windoweffects->Reset();
            }
            else if(windoweffects->GetCurrentEffect() == WindowEffects::NOTHING){
                windoweffects->FadeToBlack(1.5);
                Player::player->SetInvincibility(true);
                Player::player->KeepStill(true);
            }
            else if(windoweffects->IsBlack()){
                Player::player->KeepStill(false);
                std::vector<std::string> files = tilemap->GetPortalFiles(nextMap);
                Vec2 portalloc = tilemap->GetPortalLoc(nextMap);
                ClearMobs();
                tilemap->Load(files[0]);
                tilemap->Start();
                tilemap->LoadInfo(files[1]);

                GameData::checkpointMap = files[0];
                GameData::checkpointMapInfo = files[1];
                GameData::checkpointPos = portalloc;
                Player::player->MovePlayer(portalloc.x,portalloc.y);
                Player::player->SetInvincibility(false);
                changingMap = false;
                windoweffects->FadeFromBlack(1);
            }
        }
        else if((tilemapLoc != -1000) && (tilemapLoc != (nextMap -1))){
            changingMap = false;
        }
    }
}

void StageState::UpdateHP(){
    std::stringstream playerhptext;
    playerhptext << Player::player->GetLife();
    this->playerhp->SetText(playerhptext.str());
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