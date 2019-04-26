#include "../include/Component.h"


Component::Component(GameObject& associated) : associated(associated){
    freeze = false;
    freezetime = 0;
    freezetimer = new Timer();
}

Component::~Component() {
    delete freezetimer;
}

void Component::KeepStill(bool freeze,float time) {
    this->freeze = freeze;
    if((time > 0) && (freezetime <= 0)){
        freezetime = time;
        freezetimer->Delay(0);
    }
    if(!freeze){
        freezetime = 0;
        freezetimer->Restart();
    }
}

bool Component::GetFreeze(){
    return freeze;
}

void Component::UpdateFreeze(float dt) {
    if(freezetimer->Started()){
        freezetimer->Update(dt);
        if(freezetimer->Get() >= freezetime){
            freeze = false;
            freezetime = 0;
            freezetimer->Restart();
        }
    }
}