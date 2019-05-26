#include "../include/PauseState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"

PauseState::PauseState(){
}

PauseState::~PauseState(){
}

void PauseState::LoadAssets(){

}

void PauseState::Update(float dt){
    InputManager *input = &(InputManager::GetInstance());

	if(input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(ESCAPE_KEY)){
        GameObject* quittohomeObj = new GameObject();
        Event *quittohome = new Event(*quittohomeObj,Event::QUITTOMAINSCREEN);
        quittohomeObj->AddComponent(quittohome);
        GameData::events.push(quittohome);
        popRequested = true;
    }
    if(input->KeyPress(SDLK_RETURN)){
        popRequested = true;
    }
}

void PauseState::Render(){
}

void PauseState::Start(){
}

void PauseState::Resume(){

}

void PauseState::Pause(){

}