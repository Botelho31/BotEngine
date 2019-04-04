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
    texttimer =  new Timer();

    GameObject *titleobj =  new GameObject();
    Sprite *title = new Sprite(*titleobj,"assets/img/title.jpg");
    titleobj->box.x = 0;
    titleobj->box.y = 0;
    titleobj->AddComponent(title);
    AddObject(titleobj);

    GameObject *textObj = new GameObject();
    Text *text = new Text(*textObj,"assets/font/Callmemaybe.ttf",20,Text::BLENDED,"Press SPACE to play",{0,0,0});
    textObj->box.Transform(512 - textObj->box.w/2,500);
    textObj->AddComponent(text);
    AddObject(textObj);

}

TitleState::~TitleState(){

}

void TitleState::LoadAssets(){

}

void TitleState::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
    texttimer->Update(dt);
    if(input->KeyPress(ESCAPE_KEY) || input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(SDLK_SPACE)){
        Game::GetInstance().Push(new StageState());
    }

}

void TitleState::Render(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        Component *component = objectArray[i]->GetComponent("Text");
        if(component){
            if(texttimer->Get() > 0.4){
                if(texttimer->Get() > 0.8){
                    texttimer->Restart();
                }
            }else{
                objectArray[i]->Render();
            }
        }else{
            objectArray[i]->Render();
        }
    }
}

void TitleState::Start(){

}

void TitleState::Resume(){
    Camera::pos.Transform(0,0);
}

void TitleState::Pause(){

}