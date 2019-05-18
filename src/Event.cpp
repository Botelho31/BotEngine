#include "../include/Event.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"

Event::Event(GameObject& associated,Rect box,EventType eventType,std::string tileMap,std::string tileMapInfo,Vec2 portalloc) : 
    Component(associated){
    associated.box = box;
    this->tileMap = tileMap;
    this->tileMapInfo = tileMapInfo;
    this->portalLoc = portalloc;

    this->eventType = eventType;

    this->processing = false;
    
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

Event::~Event(){
}

void Event::Update(float dt){
}

bool Event::IsProcessing(){
    return processing;
}

void Event::SetProcessing(bool processing){
    this->processing = processing;
}

Event::EventType Event::GetType(){
    return eventType;
}

Rect Event::GetBox(){
    return associated.box;
}

Vec2 Event::GetPortalLoc(){
    return portalLoc;
}

std::string Event::GetTileMap(){
    return tileMap;
}

std::string Event::GetTileMapInfo(){
    return tileMapInfo;
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