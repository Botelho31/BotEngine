#include "../include/GameOverState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"
#include "../include/MousePointer.h"
#include "../include/Collider.h"
#include "../include/Collision.h"
#include "../include/Rotating.h"
#include  "../include/Sound.h"

GameOverState::GameOverState(StageState *stagestate){

    this->stagestate = stagestate;
    // GameObject *screenObj =  new GameObject();
    // Sprite *screen = new Sprite(*screenObj,"assets/img/HUD/gameoverscreen.png");
    // screenObj->box.x = 0 + Camera::pos.x;
    // screenObj->box.y = 0 + Camera::pos.y;
    // screenObj->AddComponent(screen);
    // AddObject(screenObj);

    GameObject *circleobj =  new GameObject();
    Sprite *circle = new Sprite(*circleobj,"assets/img/HUD/assetbranco.png");
    circleobj->box.x = -200 + Camera::pos.x;
    circleobj->box.y = -250 + Camera::pos.y;
    Rotating *rotating1 = new Rotating(*circleobj,-1);
    circleobj->AddComponent(rotating1);
    circleobj->AddComponent(circle);
    AddObject(circleobj);

    GameObject *circle2obj =  new GameObject();
    Sprite *circle2 = new Sprite(*circle2obj,"assets/img/HUD/assetbranco2.png");
    circle2obj->box.x = 1362 + Camera::pos.x;
    circle2obj->box.y = 350  + Camera::pos.y;
    Rotating *rotating2 = new Rotating(*circle2obj,1);
    circleobj->AddComponent(rotating2);
    circle2obj->AddComponent(circle2);
    AddObject(circle2obj);

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
    this->returnToMenuButton = new Button(*returnToMenuObj,{1266 + Camera::pos.x,750 + Camera::pos.y},"assets/img/buttons/normal/menu.png","assets/img/buttons/highlighted/menu.png","assets/img/buttons/selected/retornar.png");
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
        PlaySoundEffect("assets/audio/effects/select.ogg");
        popRequested = true;
    }
    if(returnToMenuButton->isSelected()){
        PlaySoundEffect("assets/audio/effects/select.ogg");
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

void GameOverState::PlaySoundEffect(std::string file,int times){
    GameObject *effectObj = new GameObject();
    Sound *sound = new Sound(*effectObj,file);
    sound->Play(times,true);
    effectObj->AddComponent(sound);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
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