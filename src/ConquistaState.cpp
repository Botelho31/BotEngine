#include "../include/ConquistaState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"
#include "../include/MousePointer.h"
#include "../include/Collider.h"
#include "../include/Collision.h"

ConquistaState::ConquistaState(StageState *stagestate,std::string file,Vec2 pos){
    this->stagestate =stagestate;

    GameObject *screenObj =  new GameObject();
    Sprite *screen = new Sprite(*screenObj,"assets/img/HUD/fundoconquistas.png");
    screenObj->box.x = Camera::pos.x;
    screenObj->box.y = Camera::pos.y;
    screenObj->AddComponent(screen);
    AddObject(screenObj);

    GameObject *screen2Obj =  new GameObject();
    Sprite *screen2 = new Sprite(*screen2Obj,file);
    screen2Obj->box.x = pos.x + Camera::pos.x;
    screen2Obj->box.y = pos.y + Camera::pos.y;
    screen2Obj->AddComponent(screen2);
    AddObject(screen2Obj);
}

ConquistaState::~ConquistaState(){
    stagestate = nullptr;
}

void ConquistaState::LoadAssets(){

}

void ConquistaState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    State::UpdateArray(dt);
    if(input->KeyPress(SDLK_SPACE)){
        popRequested = true;
    }
}

void ConquistaState::Render(){
    stagestate->Render();
    State::RenderArray();
}

void ConquistaState::Start(){
}

void ConquistaState::Resume(){

}

void ConquistaState::Pause(){

}