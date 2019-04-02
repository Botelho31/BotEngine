#include "../include/State.h"
#include "../include/Face.h"
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

State::State(){
    quitRequested = false;
    started = false;
    music = new Music("assets/audio/stageState.ogg");
    music->Play();

	GameObject *background = new GameObject(this);
    bg = new Sprite(*background,"assets/img/ocean.jpg");
    CameraFollower *camerafollower = new CameraFollower(*background);
	background->AddComponent(bg);
    background->AddComponent(camerafollower);
	objectArray.emplace_back(background);

	GameObject *tileObj = new GameObject(this);
    GameObject *tilesetObj = new GameObject(this);
	TileSet *tileSet = new TileSet(tilesetObj,64,64,"assets/img/tileset.png");
	TileMap *tileMap = new TileMap(*tileObj,"assets/map/tileMap.txt",tileSet);
	tileObj->box.x = 0;
	tileObj->box.y = 0;
	tileObj->AddComponent(tileMap);
	objectArray.emplace_back(tileObj);

    GameObject *alienObj = new GameObject(this);
    Alien *alien = new Alien(*alienObj,5);
    alienObj->box.Transform(512 - (alienObj->box.w/2),300 - (alienObj->box.h/2));
    alienObj->AddComponent(alien);
    objectArray.emplace_back(alienObj);

    GameObject *penguinObj = new GameObject(this);
    PenguinBody *penguin = new PenguinBody(*penguinObj);
    penguinObj->box.Transform(704,640);
    penguinObj->AddComponent(penguin);
    objectArray.emplace_back(penguinObj);
    Camera::Follow(penguinObj);

}

State::~State(){
	if(music != nullptr){
		delete music;
	}
	std::cout << "Cleared "<< objectArray.size() << " Objects" << std::endl;
    objectArray.clear();
}

void State::Start(){
    LoadAssets();
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Start();
    }
    started = true;
}

std::weak_ptr<GameObject> State::AddObject(GameObject *go){
    std::shared_ptr<GameObject> object(go);
    objectArray.push_back(object);
    if(started){
        object->Start();
    }
    std::weak_ptr<GameObject> weakptr = object;
    return weakptr;
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject *go){
    for(unsigned int i = 0; i < objectArray.size();i++){
        if(objectArray[i].get() == go){
            std::weak_ptr<GameObject> weakptr = objectArray[i];
            return weakptr;
        }
    }
    std::weak_ptr<GameObject> weakptr;
    return weakptr;
}

void State::LoadAssets(){

}

void State::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    Camera::Update(dt);
	if(input->IsKeyDown(ESCAPE_KEY) || input->QuitRequested()){
		quitRequested = true;
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

void State::Render(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component = objectArray[i]->GetComponent("TileMap");
        if(component){
            objectArray[i]->box.x = Camera::pos.x;
            objectArray[i]->box.y = Camera::pos.y;
        }
        objectArray[i]->Render();
    }
}

bool State::QuitRequested(){
    return quitRequested;
}