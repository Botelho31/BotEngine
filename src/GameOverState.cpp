#include "../include/GameOverState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"
#include "../include/MousePointer.h"
#include "../include/Collider.h"
#include "../include/Collision.h"

GameOverState::GameOverState(StageState *stagestate){

    this->stagestate = stagestate;
    // GameObject *screenObj =  new GameObject();
    // Sprite *screen = new Sprite(*screenObj,"assets/img/HUD/gameoverscreen.png");
    // screenObj->box.x = 0 + Camera::pos.x;
    // screenObj->box.y = 0 + Camera::pos.y;
    // screenObj->AddComponent(screen);
    // AddObject(screenObj);

    GameObject *gameoverObj =  new GameObject();
    Sprite *gameover = new Sprite(*gameoverObj,"assets/img/HUD/gameover.png");
    gameoverObj->box.x = 395 + Camera::pos.x;
    gameoverObj->box.y = 400 + Camera::pos.y;
    gameoverObj->AddComponent(gameover);
    AddObject(gameoverObj);

    GameObject *mousePointerObj = new GameObject();
    MousePointer *mousepointer = new MousePointer(*mousePointerObj);
    mousePointerObj->AddComponent(mousepointer);
    AddObject(mousePointerObj);

    GameObject *continueObj = new GameObject();
    this->continueButton = new Button(*continueObj,{645 + Camera::pos.x,750 + Camera::pos.y},"assets/img/buttons/normal/sofrer.png","assets/img/buttons/highlighted/sofrer.png","assets/img/buttons/selected/sofrer.png");
    continueObj->AddComponent(continueButton);
    AddObject(continueObj);

    GameObject *returnToMenuObj = new GameObject();
    this->returnToMenuButton = new Button(*returnToMenuObj,{1266 + Camera::pos.x,750 + Camera::pos.y},"assets/img/buttons/normal/menu.png","assets/img/buttons/highlighted/menu.png","assets/img/buttons/selected/menu.png");
    returnToMenuObj->AddComponent(returnToMenuButton);
    AddObject(returnToMenuObj);
}

GameOverState::~GameOverState(){
    stagestate = nullptr;
}

void GameOverState::LoadAssets(){

}

void GameOverState::Update(float dt){
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

void GameOverState::Render(){
    stagestate->Render();
    State::RenderArray();
}

void GameOverState::Start(){
}

void GameOverState::Resume(){

}

void GameOverState::Pause(){

}