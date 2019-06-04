#include "../include/TitleState.h"
#include "../include/GameObject.h"
#include "../include/Sprite.h"
#include "../include/InputManager.h"
#include "../include/StageState.h"
#include "../include/Camera.h"
#include "../include/Text.h"
#include "../include/MousePointer.h"
#include "../include/Collider.h"
#include "../include/Collision.h"
#include "../include/Button.h"
#include "../include/MenuState.h"

TitleState::TitleState(){
    quitRequested = false;
    popRequested = false;

    GameObject *titleobj =  new GameObject();
    Sprite *title = new Sprite(*titleobj,"assets/img/titlescreen2.png");
    titleobj->box.x = 0;
    titleobj->box.y = 0;
    titleobj->AddComponent(title);
    AddObject(titleobj);

    GameObject *mousePointerObj = new GameObject();
    MousePointer *mousepointer = new MousePointer(*mousePointerObj);
    mousePointerObj->AddComponent(mousepointer);
    AddObject(mousePointerObj);

    GameObject *textObj = new GameObject();
    Sprite* text = new Sprite(*textObj,"assets/img/titlePagetext.png");
    textObj->box.SetCenter({960,975});
    textObj->AddComponent(text);
    AddObject(textObj);

    GameObject *playButtonObj = new GameObject();
    this->playButton = new Button(*playButtonObj,{970,830},"assets/img/buttons/normal/playButton.png","assets/img/buttons/highlighted/playButton.png","assets/img/buttons/selected/playButton.png");
    playButtonObj->AddComponent(playButton);
    AddObject(playButtonObj);
}

TitleState::~TitleState(){
    objectArray.clear();
}

void TitleState::LoadAssets(){

}

void TitleState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    State::UpdateArray(dt);

    if(playButton->isSelected()){
        Game::GetInstance().Push(new MenuState());
    }
    else if(input->KeyPress(SDLK_SPACE)){
        Game::GetInstance().Push(new MenuState());
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

void TitleState::Render(){
    State::RenderArray();
}

void TitleState::Start(){

}

void TitleState::Resume(){
    Camera::pos.Transform(0,0);
    playButton->Reset();
}

void TitleState::Pause(){

}