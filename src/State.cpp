#include "State.h"

State::State(){
    popRequested = false;
    quitRequested = false;
    started = false;
}

State::~State(){
    objectArray.clear();
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
	if(input->IsKeyDown(ESCAPE_KEY) || input->QuitRequested()){
		quitRequested = true;
	}
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Update(dt);
    }
    for(unsigned int i = 0; i < objectArray.size();i++){
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