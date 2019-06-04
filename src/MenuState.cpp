#include "../include/MenuState.h"
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

MenuState::MenuState(){
    quitRequested = false;
    popRequested = false;

    GameObject *titleobj =  new GameObject();
    Sprite *title = new Sprite(*titleobj,"assets/img/titlescreenreference.png");
    titleobj->box.x = 0;
    titleobj->box.y = 0;
    titleobj->AddComponent(title);
    AddObject(titleobj);

    GameObject *mousePointerObj = new GameObject();
    MousePointer *mousepointer = new MousePointer(*mousePointerObj);
    mousePointerObj->AddComponent(mousepointer);
    AddObject(mousePointerObj);

    GameObject *newgameButtonObj = new GameObject();
    this->newgameButton = new Button(*newgameButtonObj,{0,0},"assets/img/normalPlayButton.png","assets/img/highlightedPlayButton.png","assets/img/selectedPlayButton.png");
    newgameButtonObj->box.SetCenter({970,830});
    newgameButtonObj->AddComponent(newgameButton);
    AddObject(newgameButtonObj);

    GameObject *loadButtonObj = new GameObject();
    this->loadButton = new Button(*loadButtonObj,{0,0},"assets/img/normalPlayButton.png","assets/img/highlightedPlayButton.png","assets/img/selectedPlayButton.png");
    loadButtonObj->box.SetCenter({970,830});
    loadButtonObj->AddComponent(loadButton);
    AddObject(loadButtonObj);

    GameObject *optionsButtonObj = new GameObject();
    this->optionsButton = new Button(*optionsButtonObj,{0,0},"assets/img/normalPlayButton.png","assets/img/highlightedPlayButton.png","assets/img/selectedPlayButton.png");
    optionsButtonObj->box.SetCenter({970,830});
    optionsButtonObj->AddComponent(optionsButton);
    AddObject(optionsButtonObj);

    GameObject *quitButtonObj = new GameObject();
    this->quitButton = new Button(*quitButtonObj,{0,0},"assets/img/normalPlayButton.png","assets/img/highlightedPlayButton.png","assets/img/selectedPlayButton.png");
    quitButtonObj->box.SetCenter({970,830});
    quitButtonObj->AddComponent(quitButton);
    AddObject(quitButtonObj);
}

MenuState::~MenuState(){
    objectArray.clear();
}

void MenuState::LoadAssets(){

}

void MenuState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    State::UpdateArray(dt);

    if(newgameButton->isSelected()){

    }
    else if(loadButton->isSelected()){
        Game::GetInstance().Push(new StageState());
    }
    else if(optionsButton->isSelected()){

    }
    else if(quitButton->isSelected()){
        popRequested = true;
    }
    else if(input->KeyPress(SDLK_SPACE)){
        Game::GetInstance().Push(new StageState());
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

void MenuState::Render(){
    State::RenderArray();
}

void MenuState::Start(){

}

void MenuState::Resume(){
    Camera::pos.Transform(0,0);
    newgameButton->Reset();
    loadButton->Reset();
    optionsButton->Reset();
    quitButton->Reset();
}

void MenuState::Pause(){

}