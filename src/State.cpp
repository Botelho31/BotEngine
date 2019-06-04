#include "State.h"

State::State(){
    popRequested = false;
    quitRequested = false;
    started = false;
    tilemap = nullptr;
}

State::~State(){
    tilemap = nullptr;
    objectArray.clear();
}

std::weak_ptr<GameObject> State::AddObject(GameObject *go,int place){
    std::shared_ptr<GameObject> object(go);
    if((place == 0) || (place >= objectArray.size())){
        objectArray.push_back(object);
    }else{
        objectArray.insert(objectArray.begin() + place,object);
    }
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

int State::GetObjectPlaceAtLine(std::string componenttype){
    for(unsigned int i = 0; i < objectArray.size();i++){
        if(objectArray[i].get()->GetComponent(componenttype)){
            return i;
        }
    }
    return -1;
}

int State::GetNumberOf(std::string type){
    int count = 0;
    for(int i = 0;i < objectArray.size();i++){
        if(objectArray[i]->GetComponent(type)){
            count ++;
        }
    }
    return count;
}

TileMap* State::GetTileMap(){
    return tilemap;
}

bool State::PopRequested(){
    return popRequested;
}

bool State::QuitRequested(){
    return quitRequested;
}


void State::StartArray(){
    LoadAssets();
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Start();
    }
    started = true;
}

void State::UpdateArray(float dt){
	InputManager *input = &(InputManager::GetInstance());
	if(input->KeyPress(ESCAPE_KEY) || input->QuitRequested()){
		quitRequested = true;
	}
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Update(dt);
    }
    for(int i = (objectArray.size() - 1); i >= 0;--i){
        if(objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin() + i);
        }
    }
}

void State::RenderArray(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Render();
    }
}