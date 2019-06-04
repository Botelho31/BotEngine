#include "../include/TitleState.h"
#include "../include/GameObject.h"
#include "../include/Sprite.h"
#include "../include/InputManager.h"
#include "../include/StageState.h"
#include "../include/Camera.h"
#include "../include/Text.h"
#include "../include/MousePointer.h"

TitleState::TitleState(){
    quitRequested = false;
    popRequested = false;

    GameObject *titleobj =  new GameObject();
    Sprite *title = new Sprite(*titleobj,"assets/img/titlescreen.png");
    titleobj->box.x = 0;
    titleobj->box.y = 0;
    titleobj->AddComponent(title);
    AddObject(titleobj);

    GameObject *mousePointerObj = new GameObject();
    MousePointer *mousepointer = new MousePointer(*mousePointerObj);
    mousePointerObj->AddComponent(mousepointer);
    AddObject(mousePointerObj);
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
    State::UpdateArray(dt); 
}

void TitleState::Render(){
    State::RenderArray();
}

void TitleState::Start(){

}

void TitleState::Resume(){
    Camera::pos.Transform(0,0);
}

void TitleState::Pause(){

}