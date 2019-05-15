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
    mapcollision = false;
    windoweffects = new WindowEffects();

    //Loads the background music;
    backgroundMusic = new Music("assets/audio/stageState.ogg");
    // backgroundMusic->Play();
    
    //Loads the background
	GameObject *background = new GameObject();
    Sprite *bg = new Sprite(*background,"assets/img/fundobranco.jpg");
    CameraFollower *camerafollower = new CameraFollower(*background);
    bg->SetScaleX(4,4);
	background->AddComponent(bg);
    background->AddComponent(camerafollower);
	objectArray.emplace_back(background);

    //LOADS THE GAME DATA;
    GameData::LoadGame();
	GameObject *tileObj = new GameObject();
    GameObject *tilesetObj = new GameObject();
	this->tileset = new TileSet(tilesetObj,32,32,"assets/img/basictiletest.png");
	this->tilemap = new TileMap(*tileObj,GameData::checkpointMap,tileset);
	tileObj->box.x = 0;
	tileObj->box.y = 0;
	tileObj->AddComponent(tilemap);
	objectArray.emplace_back(tileObj);

    GameObject *playerObj = new GameObject();
    Player *player = new Player(*playerObj);
    player->SetHealth(GameData::savePlayerHealth);
    playerObj->box.SetCenter(GameData::savePlayerPos);
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
    //FINISHES LOADING THE PLAYER DATA
}

StageState::~StageState(){
    playerhp = nullptr;
    delete windoweffects;
    if(backgroundMusic){
        delete backgroundMusic;
    }

    std::cout << "Saving Game" << std::endl;
    GameData::SaveGame();
    std::cout << std::endl;

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
        if(objectArray[i]->GetPhysics() && !mapcollision){

        }else{
            objectArray[i]->Update(dt);
        }
    }

    //Expands the tile colliders to their maximum then starts interpreting tilecollision
    if(!mapcollision){
        ExpandTileColliders();
    }else{
        for(unsigned int i = 0; i < objectArray.size();i++){
            Component *component1 = objectArray[i]->GetComponent("Collider");
            if((component1) && ((i + 1) < objectArray.size())){
                Collider *collider1 = dynamic_cast<Collider*>(component1);
                for(unsigned int j = i + 1; j < objectArray.size();j++){
                    Component *component2 = objectArray[j]->GetComponent("Collider");
                    if(component2){
                        Collider *collider2 = dynamic_cast<Collider*>(component2);
                        if(Collision::IsColliding(collider1->box,collider2->box,ToPI(objectArray[i]->angleDeg),ToPI(objectArray[j]->angleDeg))){
                            objectArray[i]->NotifyCollision(*objectArray[j]);
                            objectArray[j]->NotifyCollision(*objectArray[i]);
                        }
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
            tilemap->Start();
            mapcollision = false;
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

void StageState::ExpandTileColliders(){
    bool mapcollisionloaded = true;
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component1 = objectArray[i]->GetComponent("TileCollider");
        if(component1){
            TileCollider *tilecollider1 = dynamic_cast<TileCollider*>(component1);
            if(!tilecollider1->maxX || !tilecollider1->maxY){
                mapcollisionloaded = false;
            }
            if((i + 1) < objectArray.size()){
                for(unsigned int j = i + 1; j < objectArray.size();j++){
                    Component *component2 = objectArray[j]->GetComponent("TileCollider");
                    if(component2){
                        TileCollider *tilecollider2 = dynamic_cast<TileCollider*>(component2);
                        if(Collision::IsColliding(tilecollider1->box,tilecollider2->box,ToPI(objectArray[i]->angleDeg),ToPI(objectArray[j]->angleDeg)) && !tilecollider1->moving && !tilecollider2->moving){
                            objectArray[i]->NotifyCollision(*objectArray[j]);
                            objectArray[j]->NotifyCollision(*objectArray[i]);
                        }
                    }
                }
            }
        }
    }
    if(mapcollisionloaded){
        // std::cout << "Map Collision Loaded" << std::endl;
        mapcollision = true;
    }
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
        Physics* playerphysics = Player::player->GetPhysics();
        
        if(playerphysics->IsOutofBounds(true)){
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
                mapcollision = false;
                tilemap->LoadInfo(files[1]);

                GameData::checkpointMap = files[0];
                GameData::checkpointMapInfo = files[1];
                GameData::checkpointPos = portalloc;
                Player::player->MovePlayer(portalloc.x,portalloc.y);
                Player::player->SetInvincibility(false);
                windoweffects->FadeFromBlack(1);
                changingMap = false;
            }
        }
        else if((!playerphysics->IsOutofBounds()) && (tilemapLoc != (nextMap -1))){
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
    this->tilemap->LoadInfo(GameData::checkpointMapInfo);
}

void StageState::Pause(){

}

void StageState::Resume(){

}