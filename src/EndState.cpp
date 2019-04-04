#include "../include/EndState.h"
#include "../include/GameData.h"
#include "../include/Sprite.h"
#include "../include/Text.h"
#include "../include/InputManager.h"
#include "../include/TitleState.h"

EndState::EndState(){
    GameObject *bgObj = new GameObject();
    if(GameData::playerVictory){
        Sprite *bg = new Sprite(*bgObj,"assets/img/win.jpg");
        backgroundMusic = new Music("assets/audio/endStateWin.ogg");
        backgroundMusic->Play();
        bgObj->box.Transform(0,0);
        bgObj->AddComponent(bg);
        AddObject(bgObj);
    }else{  
        Sprite *bg = new Sprite(*bgObj,"assets/img/lose.jpg");
        backgroundMusic = new Music("assets/audio/endStateLose.ogg");
        backgroundMusic->Play();
        bgObj->box.Transform(0,0);
        bgObj->AddComponent(bg);
        AddObject(bgObj);
    }
    GameObject* textObj = new GameObject();
    Text *text = new Text(*textObj,"assets/font/Callmemaybe.ttf",20,Text::BLENDED,"Press Space to Play again or ESC to Quit",{0,0,0});
    textObj->box.Transform(512 - textObj->box.w/2,500);
    textObj->AddComponent(text);
    AddObject(textObj);
}

EndState::~EndState(){
    if(backgroundMusic){
        delete backgroundMusic;
    }
}

void EndState::LoadAssets(){
    
}

void EndState::Update(float dt){
    InputManager *input = &(InputManager::GetInstance());
    if((input->KeyPress(ESCAPE_KEY)) || (input->QuitRequested())){
		quitRequested = true;
	}
    if(input->KeyPress(SDLK_SPACE)){
        popRequested = true;
        Game::GetInstance().Push(new TitleState());
    }
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Update(dt);
    }
}

void EndState::Render(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Render();
    }
}

void EndState::Start(){

}

void EndState::Resume(){
    
}

void EndState::Pause(){
    
}