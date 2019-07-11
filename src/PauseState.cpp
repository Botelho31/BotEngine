#include "../include/PauseState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"
#include "../include/MousePointer.h"
#include "../include/Collider.h"
#include "../include/Collision.h"

PauseState::PauseState(StageState *stagestate){
    this->stagestate =stagestate;
    this->screenObj =  new GameObject();
    Sprite *screen = new Sprite(*screenObj,"assets/img/HUD/pausescreen.png");
    screenObj->box.x = 645 + Camera::pos.x;
    screenObj->box.y = 255 + Camera::pos.y;
    screenObj->AddComponent(screen);
    AddObject(screenObj);

    GameObject *mousePointerObj = new GameObject();
    MousePointer *mousepointer = new MousePointer(*mousePointerObj);
    mousePointerObj->AddComponent(mousepointer);
    AddObject(mousePointerObj);

    GameObject *continueObj = new GameObject();
    this->continueButton = new Button(*continueObj,{945 + Camera::pos.x,490 + Camera::pos.y},"assets/img/buttons/normal/continuar.png","assets/img/buttons/highlighted/continuar.png","assets/img/buttons/selected/continuar.png");
    continueObj->AddComponent(continueButton);
    AddObject(continueObj);

    GameObject *returnToMenuObj = new GameObject();
    this->returnToMenuButton = new Button(*returnToMenuObj,{945 + Camera::pos.x,637 + Camera::pos.y},"assets/img/buttons/normal/retorne.png","assets/img/buttons/highlighted/retorne.png","assets/img/buttons/selected/retorne.png");
    returnToMenuObj->AddComponent(returnToMenuButton);
    AddObject(returnToMenuObj);
}

PauseState::~PauseState(){
    stagestate = nullptr;
}

void PauseState::LoadAssets(){

}

void PauseState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    State::UpdateArray(dt);
    if(continueButton->isSelected()){
        popRequested = true;
    }
    if(returnToMenuButton->isSelected()){
        popRequested = true;
        stagestate->ReturnToMenu();
    }

    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component1 = objectArray[i]->GetComponent("Collider");
        if((component1) && ((i + 1) < objectArray.size())){
            Collider *collider1 = dynamic_cast<Collider*>(component1);
            for(unsigned int j = i + 1; j < objectArray.size();j++){
                Component *component2 = objectArray[j]->GetComponent("Collider");
                if(component2){
                    Collider *collider2 = dynamic_cast<Collider*>(component2);
                    if(Collision::IsColliding(collider1->box,collider2->box,ToPI(objectArray[i]->angleDeg),ToPI(objectArray[j]->angleDeg))){
                        objectArray[i]->NotifyCollision(*objectArray[j]);
                        objectArray[j]->NotifyCollision(*objectArray[i]);
                    }
                }
            }
        }
    }
}

void PauseState::Render(){
    stagestate->Render();
    State::RenderArray();
}

void PauseState::Start(){
}

void PauseState::Resume(){

}

void PauseState::Pause(){

}