#include "../include/StageState.h"
#include "../include/Sprite.h"
#include "../include/Vec2.h"
#include "../include/Sound.h"
#include "../include/TileSet.h"
#include "../include/TileMap.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/CameraFollower.h"
#include "../include/Alien.h"
#include "../include/PenguinBody.h"
#include "../include/Collision.h"
#include "../include/Collider.h"

StageState::StageState(){
    quitRequested = false;
    popRequested = false;
    started = false;
    backgroundMusic = new Music("assets/audio/stageState.ogg");
    backgroundMusic->Play();

	GameObject *background = new GameObject();
    Sprite *bg = new Sprite(*background,"assets/img/ocean.jpg");
    CameraFollower *camerafollower = new CameraFollower(*background);
	background->AddComponent(bg);
    background->AddComponent(camerafollower);
	objectArray.emplace_back(background);

	GameObject *tileObj = new GameObject();
    GameObject *tilesetObj = new GameObject();
	this->tileset = new TileSet(tilesetObj,64,64,"assets/img/tileset.png");
	TileMap *tileMap = new TileMap(*tileObj,"assets/map/tileMap.txt",tileset);
	tileObj->box.x = 0;
	tileObj->box.y = 0;
	tileObj->AddComponent(tileMap);
	objectArray.emplace_back(tileObj);

    GameObject *alienObj = new GameObject();
    Alien *alien = new Alien(*alienObj,2);
    alienObj->box.Transform(512 - (alienObj->box.w/2),300 - (alienObj->box.h/2));
    alienObj->AddComponent(alien);
    objectArray.emplace_back(alienObj);

    GameObject *penguinObj = new GameObject();
    PenguinBody *penguin = new PenguinBody(*penguinObj);
    penguinObj->box.Transform(704,640);
    penguinObj->AddComponent(penguin);
    objectArray.emplace_back(penguinObj);
    Camera::Follow(penguinObj);

}

StageState::~StageState(){
	if(backgroundMusic != nullptr){
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

bool StageState::QuitRequested(){
    return quitRequested;
}

void StageState::Start(){
    StartArray();
}

void StageState::Pause(){

}

void StageState::Resume(){

}