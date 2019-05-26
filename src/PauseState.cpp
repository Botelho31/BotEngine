#include "../include/PauseState.h"
#include "../include/Camera.h"

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