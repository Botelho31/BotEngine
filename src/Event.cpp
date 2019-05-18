#include "../include/Event.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"

Event::Event(GameObject& associated,Rect box,bool isPortal,std::string tileMap,std::string tileMapInfo,Vec2 portalloc) : 
    Component(associated){
    associated.box = box;
    this->isPortal = isPortal;
    this->tileMap = tileMap;
    this->tileMapInfo = tileMapInfo;
    this->portalLoc = portalloc;
    
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

Event::~Event(){
}

void Event::Update(float dt){
}


void Event::Render() {
#ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS)){
		WindowEffects::DrawBox(associated.box,ToPI(associated.angleDeg),255,122,0);
	}
#endif // DEBUG
}

void Event::NotifyCollision(GameObject& other){

}

bool Event::Is(std::string type){
    if(type == "Event"){
        return true;
    }else{
        return false;
    }
}