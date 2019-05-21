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
    showfps = false;
    windoweffects = new WindowEffects();
    changingMapTimer = new Timer();

    //Loads the background music;
    backgroundMusic = new Music("assets/audio/musicmenubel.ogg");
    // backgroundMusic->Play();
}

StageState::~StageState(){
    playerhp = nullptr;
    delete windoweffects;
    delete changingMapTimer;
    if(backgroundMusic){
        delete backgroundMusic;
    }
    
    ENDLINE
    std::cout << "Saving Game" << std::endl;
    GameData::SaveGame();

	std::cout << "Cleared "<< objectArray.size() << " Objects" << std::endl;
    objectArray.clear();
}

void StageState::LoadAssets(){

}

void StageState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    Camera::Update(dt);
    
    //FOR DEBUGGING REASONS SPAWNS MINION ON 6
    #ifdef DEBUG
        Vec2 mousePos = Vec2(input->GetMouseX() * 2 + Camera::pos.x,input->GetMouseY() * 2 + Camera::pos.y);
        if(input->KeyPress(SDLK_6)){
            GameObject *minionObj = new GameObject();
            Minion *minion = new Minion(*minionObj);
            minionObj->AddComponent(minion);
            Vec2  minionpos = mousePos;
            minionObj->box.SetCenter(minionpos);
            AddObject(minionObj);
        }
        if(input->KeyPress(SDLK_2)){
            if(showfps){
                showfps = false;
            }else{
                showfps = true;
            }
        }
        if(showfps){
            std::stringstream fpsstring;
            fpsstring << (int)(1/dt);
            this->fps->SetText(fpsstring.str());
            // std::cout << objectArray.size() << std::endl;
        }else{
            this->fps->SetText("");
        }
    //FOR DEBUGGING REASONS SPAWNS MINION ON 6
    #endif

	if(input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(ESCAPE_KEY)){
        popRequested = true;
    }
    for(unsigned int i = 0; i < objectArray.size();i++){
        if(mapcollision){
            objectArray[i]->Update(dt);
        }else{
            if(!objectArray[i]->GetPhysics()){
                objectArray[i]->Update(dt);
            }
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
    
    HandleEvents(dt);    //HANDLE TILEMAP EXCHANGE

    //Checks if player is completely in bounds
    if(changingMapTimer->Started()){
        changingMapTimer->Update(dt);
        if(Player::player){
            Physics *physics = Player::player->GetPhysics();
            if(!physics->IsOutofBounds()){
                changingMap = false;
                changingMapTimer->Restart();
                ENDLINE
                std::cout << "Changed Map" << std::endl;
            }
        }
        if(changingMapTimer->Get() > 0.5){
            changingMapTimer->Restart();
            changingMap = false;
            ENDLINE
            std::cout << "Changed Map" << std::endl;
        }
    }
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
            if((!tilecollider1->maxX || !tilecollider1->maxY || !tilecollider1->adjusted) && !tilecollider1->moving){
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
        ENDLINE
        std::cout << "Map Collision Loaded" << std::endl;
        mapcollision = true;
        changingMapTimer->Update(0);
        windoweffects->FadeFromBlack(1.5);
    }
}

void StageState::ClearMobs(){
    for(int i = (objectArray.size() - 1); i >= 0; --i){
        Component *component1 = objectArray[i]->GetComponent("HitBox");
        Component *component2 = objectArray[i]->GetComponent("Minion");
        Component *component3 = objectArray[i]->GetComponent("MovingTile");
        Component *component4 = objectArray[i]->GetComponent("TileCollider");
        Component *component5 = objectArray[i]->GetComponent("DeadBody");
        Component *component6 = objectArray[i]->GetComponent("Event");
        if(component1){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component2){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component3){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component4){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component5){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component6){
            objectArray.erase(objectArray.begin() + i);
        }
    }
}

void StageState::HandleEvents(float dt){

    if(!GameData::events.empty()){
        //TILEMAP EXCHANGE
        if(GameData::events.front()->GetType() == Event::PORTAL){
            Physics* playerphysics = Player::player->GetPhysics();
            Rect playerbox = playerphysics->GetCollider()->box;
            Rect eventbox = GameData::events.front()->GetBox();
            changingMap = true;

            if((!playerphysics->IsOutofBounds()) && (!Collision::IsColliding(playerbox,eventbox,Player::player->GetPlayerAngle(),0))){
                changingMap = false;
                GameData::events.front()->SetProcessing(false);
                GameData::events.pop();
            }
            else if(playerphysics->IsOutofBounds(true)){
                if(windoweffects->GetCurrentEffect() == WindowEffects::FADEFROMBLACK){
                    windoweffects->Reset();
                    Player::player->SetInvincibility(true);
                    Player::player->KeepStill(true);
                }
                else if(windoweffects->GetCurrentEffect() == WindowEffects::NOTHING){
                    windoweffects->FadeToBlack(1.5);
                    Player::player->SetInvincibility(true);
                    Player::player->KeepStill(true);
                }
                else if(windoweffects->IsBlack()){
                    mapcollision = false;

                    Player::player->KeepStill(false);
                    std::string tileMapFile = GameData::events.front()->GetTileMap();
                    std::string tileMapInfoFile = GameData::events.front()->GetTileMapInfo();
                    Vec2 portalloc = GameData::events.front()->GetPortalLoc();
                    GameData::events.pop();

                    ClearMobs();
                    tilemap->Load(tileMapFile);
                    tilemap->LoadTileColliders();
                    tilemap->LoadInfo(tileMapInfoFile);

                    GameData::checkpointMap = tileMapFile;
                    GameData::checkpointMapInfo = tileMapInfoFile;
                    GameData::checkpointPos = portalloc;
                    GameData::checkpointPosSpeed = Player::player->GetSpeed();
                    Player::player->MovePlayer(portalloc.x,portalloc.y);
                    Player::player->SetInvincibility(false);
                }
            }
        }
        else if(GameData::events.front()->GetType() == Event::PLAYERDEATH){
            GameData::events.front()->Update(dt);
            if(GameData::events.front()->IsEventTimerOver()){
                if(!windoweffects->Drawing()){
                    windoweffects->FadeToBlack(2);
                }
                if(windoweffects->IsBlack()){
                    mapcollision = false;
                    changingMap = true;
                    Camera::UnFollow();
                    ClearMobs();
                    tilemap->Load(GameData::checkpointMap);
                    tilemap->LoadTileColliders();
                    tilemap->LoadInfo(GameData::checkpointMapInfo);
                    Player::player->MovePlayer(GameData::checkpointPos.x,GameData::checkpointPos.y,false);
                    if(GameData::checkpointPosSpeed.y < -100){
                        GameData::checkpointPosSpeed.y = -900;
                    }
                    Player::player->KeepStill(false);
                    Player::player->Reset(GameData::checkpointPosSpeed);
                    GameData::playerAlive = true;
                    GameData::events.pop();
                    Camera::Follow(Player::player->GetAssociated());
                }
            }
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
    windoweffects->FadeToBlack(0.5);
    
    //LOADS THE GAME DATA
    GameData::LoadGame();

    //Loads the background
	GameObject *background = new GameObject();
    Sprite *bg = new Sprite(*background,"assets/img/fundobranco.jpg");
    CameraFollower *camerafollower = new CameraFollower(*background);
    bg->SetScaleX(4,4);
	background->AddComponent(bg);
    background->AddComponent(camerafollower);
	objectArray.emplace_back(background);

    //Loads the tilemap
	GameObject *tileObj = new GameObject();
    GameObject *tilesetObj = new GameObject();
    std::vector<std::string> tilesetfiles;
    tilesetfiles.push_back("assets/img/TileSetTest7.png");
    tilesetfiles.push_back("assets/img/backgroundtest.jpg");
	this->tileset = new TileSet(tilesetObj,50,50,tilesetfiles);
	this->tilemap = new TileMap(*tileObj,GameData::checkpointMap,tileset);
	tileObj->box.x = 0;
	tileObj->box.y = 0;
	tileObj->AddComponent(tilemap);
	objectArray.emplace_back(tileObj);

    //Loads the player
    GameObject *playerObj = new GameObject();
    Player *player = new Player(*playerObj);
    player->SetHealth(GameData::savePlayerHealth);
    playerObj->box.SetCenter(GameData::savePlayerPos);
    playerObj->AddComponent(player);
    objectArray.emplace_back(playerObj);
    Camera::Follow(playerObj);

    //Loads the player HP
    GameObject *playerhpObj = new GameObject();
    std::stringstream playerLife;
    playerLife << player->GetLife();
    playerhpObj->box.x = 50;
    playerhpObj->box.y = 0;
    this->playerhp = new Text(*playerhpObj,"assets/font/Callmemaybe.ttf",100,Text::BLENDED,playerLife.str(),{0,0,0});
    CameraFollower *camerafollower2 = new CameraFollower(*playerhpObj);
    playerhpObj->AddComponent(camerafollower2);
    playerhpObj->AddComponent(playerhp);
    objectArray.emplace_back(playerhpObj);

    //Loads the FPS
    GameObject *fpsObj = new GameObject();
    fpsObj->box.x = Camera::window.x - 100;
    fpsObj->box.y = 0;
    this->fps = new Text(*fpsObj,"assets/font/Callmemaybe.ttf",50,Text::BLENDED,"0",{0,0,0});
    CameraFollower *camerafollower3 = new CameraFollower(*fpsObj);
    fpsObj->AddComponent(camerafollower3);
    fpsObj->AddComponent(fps);
    objectArray.emplace_back(fpsObj); 
       
    //FINISHES LOADING

    StartArray();
}

void StageState::Pause(){

}

void StageState::Resume(){

}