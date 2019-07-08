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
#include "../include/FlyingMinion.h"
#include "../include/TransitionState.h"
#include "../include/MovingTile.h"
#include "../include/TileCollider.h"
#include "../include/FakeWall.h"
#include "../include/Eye.h"
#include "../include/Circle.h"
#include "../include/Light.h"
#include "../include/Boss.h"
#include "../include/PauseState.h"
#include "../include/MapState.h"
#include "../include/Spike.h"
#include "../include/BackGround.h"
#include "../include/HUD.h"

bool StageState::changingMap;
bool StageState::mapcollision;
bool StageState::loadedTileColliders;
BackGround * StageState::background;

StageState::StageState(){
    quitRequested = false;
    popRequested = false;
    started = false;
    backgroundMusic = Game::GetInstance().GetMusic();
    changingMap = false;
    mapcollision = false;
    showfps = false;
    pause = false;
    windoweffects = new WindowEffects();
    changingMapTimer = new Timer();
}

StageState::~StageState(){
    delete windoweffects;
    delete changingMapTimer;
    backgroundMusic = nullptr;
    while(!GameData::events.empty()){
        GameData::events.pop();
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
        if(input->KeyPress(SDLK_7)){
            GameObject *minionObj = new GameObject();
            FlyingMinion *minion = new FlyingMinion(*minionObj);
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
        if(input->KeyPress(SDLK_3)){
            Camera::UnFollow();
        }
        if(input->KeyPress(SDLK_4)){
            Camera::ShakeScreen(1,200);
        }
        if(showfps){
            std::stringstream fpsstring;
            fpsstring << (int)(1/dt);
            this->fps->SetText(fpsstring.str());
        }else{
            this->fps->SetText("");
        }
    //FOR DEBUGGING REASONS SPAWNS MINION ON 6
    #endif

	if(input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(ESCAPE_KEY)){
         if((GameData::events.size() == 0) || ((GameData::events.size() == 1) && (GameData::events.front()->GetType() == Event::PORTAL) && (windoweffects->GetCurrentEffect() != WindowEffects::FADETOBLACK)) ){
            pause = true;
            if(GameData::events.size() == 1){
                GameData::events.pop();
            }
            GameObject* pauseeventObj = new GameObject();
            Event *pauseevent = new Event(*pauseeventObj,Event::PAUSE,0.5);
            pauseeventObj->AddComponent(pauseevent);
            GameData::events.push(pauseevent);
        }
    }

    if(input->KeyPress(SDLK_m)){
         if(!windoweffects->Drawing()){
            MapState* mapstate = new MapState();
            Game::GetInstance().Push(mapstate);
        }
    }

    for(unsigned int i = 0; i < objectArray.size();i++){
        if(mapcollision && !pause){
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
    }else if(!pause){
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

    HUDdisplay->Update(dt);
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

    loadingIcon->Update(dt);
}

void StageState::Render(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component = objectArray[i]->GetComponent("TileMap");
        if(component){
            objectArray[i]->box.x = Camera::pos.x;
            objectArray[i]->box.y = Camera::pos.y;
        }
        if(!objectArray[i]->renderAfterForeGround){
            objectArray[i]->Render();
        }
    }
    tilemap->RenderForeGround();
    for(unsigned int i = 0; i < objectArray.size();i++){
        if(objectArray[i]->renderAfterForeGround){
            objectArray[i]->Render();
        }
    }
    HUDdisplay->Render();
    windoweffects->Render();

    if(!mapcollision){
        loadingIcon->Render();
    }
}

void StageState::ExpandTileColliders(){
    bool mapcollisionloaded = true;
    bool adjustedtilecolliders = true;
    loadedTileColliders = false;

    int notloaded = 0;
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component1 = objectArray[i]->GetComponent("TileCollider");
        if(component1){
            TileCollider *tilecollider1 = dynamic_cast<TileCollider*>(component1);
            if((!tilecollider1->maxX || !tilecollider1->maxY) && !tilecollider1->moving){
                mapcollisionloaded = false;
                // tilecollider1->marked = true;
                notloaded ++;
            }else{
                // tilecollider1->marked = false;
            }
            if(!tilecollider1->adjusted){
                adjustedtilecolliders = false;
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
    #ifdef DEBUG
        std::cout << floor((1 - (float)notloaded/(float)initialtiles)*100) << "% \tOf Map Loaded" <<   std::endl;
    #endif
    if(mapcollisionloaded){
        loadedTileColliders = true;
    }
    if(mapcollisionloaded && adjustedtilecolliders){
        ENDLINE
        std::cout << "Map Collision Loaded" << std::endl;
        mapcollision = true;
        changingMapTimer->Update(0);
        ChangeMusic(GameData::currentMusic);
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
        Component *component7 = objectArray[i]->GetComponent("FakeWall");
        Component *component8 = objectArray[i]->GetComponent("Boss");
        Component *component9 = objectArray[i]->GetComponent("Eye");
        Component *component10 = objectArray[i]->GetComponent("Spike");
        Component *component11 = objectArray[i]->GetComponent("Soul");
        Component *component12 = objectArray[i]->GetComponent("FlyingMinion");
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
        else if(component7){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component8){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component9){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component10){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component11){
            objectArray.erase(objectArray.begin() + i);
        }
        else if(component12){
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
                    backgroundMusic->Stop();

                    Player::player->KeepStill(false);
                    std::string tileMapInfoFile = GameData::events.front()->GetTileMapInfo();
                    Vec2 portalloc = GameData::events.front()->GetPortalLoc();
                    GameData::events.pop();

                    ClearMobs();
                    tilemap->ExchangeMap(tileMapInfoFile);
                    this->initialtiles = TileMap::tiles.size();

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
                    tilemap->ExchangeMap(GameData::checkpointMapInfo);
                    this->initialtiles = TileMap::tiles.size();
                    Player::player->MovePlayer(GameData::checkpointPos.x,GameData::checkpointPos.y,false);
                    if(GameData::checkpointPosSpeed.y < -100){
                        GameData::checkpointPosSpeed.y = -900;
                    }
                    Player::player->KeepStill(false);
                    Player::player->SetInvincibility(false);
                    Player::player->Reset(GameData::checkpointPosSpeed);
                    GameData::playerAlive = true;
                    GameData::events.pop();
                    Camera::Follow(Player::player->GetAssociated());
                }
            }
        }
        else if(GameData::events.front()->GetType() == Event::PAUSE){
            pause = true;
            if(!windoweffects->Drawing()){
                windoweffects->DimScreen(0.5,122);
            }else{
                GameData::events.front()->Update(dt);
            }
            if(GameData::events.front()->IsEventTimerOver()){
                PauseState *pausestate = new PauseState();
                Game::GetInstance().Push(pausestate);
                GameData::events.pop();
            }
        }
        else if(GameData::events.front()->GetType() == Event::UNPAUSE){
            pause = false;
            if(windoweffects->GetCurrentEffect() == WindowEffects::DIMSCREEN){
                windoweffects->UnDimScreen(0.5,122);
            }else{
                GameData::events.front()->Update(dt);
            }
            if(GameData::events.front()->IsEventTimerOver()){
                GameData::events.pop();
            }
        }
        else if(GameData::events.front()->GetType() == Event::QUITTOMAINSCREEN){
            popRequested = true;
            GameData::events.pop();
        }
        else if(GameData::events.front()->GetType() == Event::PLAYERGETSSWORD){
            pause = true;
            if(!windoweffects->Drawing()){
                windoweffects->DimScreen(0.5,122);
            }else{
                GameData::events.front()->Update(dt);
            }
            if(GameData::events.front()->IsEventTimerOver()){
                InputManager *input = &(InputManager::GetInstance());
                if(input->KeyPress(SDLK_SPACE)){
                    windoweffects->DimScreen(0.5,0);
                    pause = false;
                    GameData::events.pop();
                }
            }
        }
        else if(GameData::events.front()->GetType() == Event::PLAYERGETSDOUBLEJUMP){
            pause = true;
            if(!windoweffects->Drawing()){
                windoweffects->DimScreen(0.5,122);
            }else{
                GameData::events.front()->Update(dt);
            }
            if(GameData::events.front()->IsEventTimerOver()){
                InputManager *input = &(InputManager::GetInstance());
                if(input->KeyPress(SDLK_SPACE)){
                    windoweffects->DimScreen(0.5,0);
                    pause = false;
                    GameData::events.pop();
                }
            }
        }
    }
}

bool StageState::ChangingMap(){
    return changingMap;
}

void StageState::Start(){
    windoweffects->FadeToBlack(0.5);
    
    //LOADS THE GAME DATA
    GameData::LoadGame();

    //Loads the background
	GameObject *backgroundObj = new GameObject();
    backgroundObj->box.Transform(0,0);
    this->background = new BackGround(*backgroundObj,"assets/img/fundos/room1.png");
	backgroundObj->AddComponent(background);
	objectArray.emplace_back(backgroundObj);

    //Loads the tilemap
	GameObject *tileObj = new GameObject();
	this->tilemap = new TileMap(*tileObj,GameData::checkpointMapInfo);
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

    //Loads the FPS
    GameObject *fpsObj = new GameObject();
    fpsObj->box.x = Camera::window.x - 100;
    fpsObj->box.y = 0;
    this->fps = new Text(*fpsObj,"assets/font/Callmemaybe.ttf",50,Text::BLENDED,"0",{0,0,0});
    CameraFollower *camerafollower3 = new CameraFollower(*fpsObj);
    fpsObj->AddComponent(camerafollower3);
    fpsObj->AddComponent(fps);
    objectArray.emplace_back(fpsObj); 

    this->HUDdisplay = new GameObject();
    HUD *hudcomp = new HUD(*HUDdisplay);
    HUDdisplay->renderAfterForeGround = true;
    HUDdisplay->AddComponent(hudcomp);

    loadingIcon = new GameObject();
    Sprite *loadingSprite = new Sprite(*loadingIcon,"assets/img/soul.png",48,0.04);
    loadingIcon->box.Transform(1700,900);
    CameraFollower *camfollower4 = new CameraFollower(*loadingIcon);
    loadingIcon->AddComponent(loadingSprite);
    loadingIcon->AddComponent(camfollower4);

    GameData::PreLoadSounds("assets/audio/info/effects.txt");

    StartArray();

    this->initialtiles = TileMap::tiles.size();
}

void StageState::Pause(){
}

void StageState::Resume(){
    GameObject* unpauseeventObj = new GameObject();
    Event *unpauseevent = new Event(*unpauseeventObj,Event::UNPAUSE,0.5);
    unpauseeventObj->AddComponent(unpauseevent);
    GameData::events.push(unpauseevent);
}

bool StageState::MapCollisionLoaded(){
    return mapcollision;
}

bool StageState::LoadedTileColliders(){
    return loadedTileColliders;
}

void StageState::ChangeBackground(std::string file,bool parallax,Vec2 scale,bool camerafollower){
    background->ChangeBackground(file,parallax,scale,camerafollower);
}

void StageState::ChangeMusic(std::string musicfile){
    backgroundMusic->Stop();
    backgroundMusic->Open(musicfile);
    backgroundMusic->Play(-1,1500,true);
}