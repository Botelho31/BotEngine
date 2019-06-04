#include "../include/TitleState.h"
#include "../include/GameObject.h"
#include "../include/Sprite.h"
#include "../include/InputManager.h"
#include "../include/StageState.h"
#include "../include/Camera.h"
#include "../include/Text.h"

TitleState::TitleState(){
    quitRequested = false;
    popRequested = false;

    GameObject *titleobj =  new GameObject();
    Sprite *title = new Sprite(*titleobj,"assets/img/titlescreen.png");
    titleobj->box.x = 0;
    titleobj->box.y = 0;
    titleobj->AddComponent(title);
    AddObject(titleobj);

}

TitleState::~TitleState(){
}

void TitleState::LoadAssets(){

}

void TitleState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    if(input->KeyPress(ESCAPE_KEY) || input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(SDLK_SPACE)){
        Game::GetInstance().Push(new StageState());
    }

}

void TitleState::Render(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Render();
    }
}

void TitleState::Start(){

}

void TitleState::Resume(){
    Camera::pos.Transform(0,0);
}

void TitleState::Pause(){

}