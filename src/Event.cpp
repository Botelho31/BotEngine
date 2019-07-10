#include "../include/Event.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"

Event::Event(GameObject& associated,EventType eventType,Rect box,std::string tileMapInfo,Vec2 portalloc) : 
    Component(associated){
    associated.box = box;
    this->tileMapInfo = tileMapInfo;
    this->portalLoc = portalloc;

    this->eventtimer = new Timer();
    this->eventduration = 0;

    this->eventType = eventType;

    this->processing = false;
    
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

Event::Event(GameObject& associated,EventType eventType,Rect box) : 
    Component(associated){
    associated.box = box;
    this->tileMapInfo = "";

    this->eventtimer = new Timer();
    this->eventduration = 0;

    this->eventType = eventType;

    this->processing = false;
    
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

Event::Event(GameObject& associated,EventType eventType,float duration) : 
    Component(associated){
    this->tileMapInfo = "";

    this->eventtimer = new Timer();
    this->eventduration = duration;

    this->eventType = eventType;

    this->processing = false;
}

Event::~Event(){
    delete eventtimer;
}

void Event::Update(float dt){
    eventtimer->Update(dt);
}

bool Event::IsEventTimerOver(){
    if(eventtimer->Get() >= eventduration){
        return true;
    }
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