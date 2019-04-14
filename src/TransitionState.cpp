#include "../include/TransitionState.h"
#include "../include/Camera.h"

TransitionState::TransitionState(float secondsToPop){
    this->poptimer = new Timer();
    this->secondsToPop = secondsToPop;
}
TransitionState::~TransitionState(){

}
void TransitionState::LoadAssets(){

}
void TransitionState::Update(float dt){
    poptimer->Update(dt);
    if(poptimer->Get() >= secondsToPop){
        popRequested = true;
    }
}
void TransitionState::Render(){

}

void TransitionState::Start(){
}
void TransitionState::Resume(){

}
void TransitionState::Pause(){

}