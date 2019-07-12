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
#include "../include/GameData.h"
#include "../include/WindowEffects.h"
#include "../include/Sound.h"

MenuState::MenuState(){
    quitRequested = false;
    popRequested = false;
    windoweffects = new WindowEffects();

    GameObject *titleobj =  new GameObject();
    Sprite *title = new Sprite(*titleobj,"assets/img/HUD/menuscreen.png");
    titleobj->box.x = 0;
    titleobj->box.y = 0;
    titleobj->AddComponent(title);
    AddObject(titleobj);

    GameObject *rebellioobj =  new GameObject();
    Sprite *rebellio = new Sprite(*rebellioobj,"assets/img/HUD/rebelliomenu.png");
    rebellioobj->box.x = 399;
    rebellioobj->box.y = -90;
    rebellioobj->AddComponent(rebellio);
    AddObject(rebellioobj);

    GameObject *mousePointerObj = new GameObject();
    MousePointer *mousepointer = new MousePointer(*mousePointerObj);
    mousePointerObj->AddComponent(mousepointer);
    AddObject(mousePointerObj);

    GameObject *newgameButtonObj = new GameObject();
    this->newgameButton = new Button(*newgameButtonObj,{969,560},"assets/img/buttons/normal/newgameButton.png","assets/img/buttons/highlighted/newgameButton.png","assets/img/buttons/selected/newgameButton.png");
    newgameButtonObj->AddComponent(newgameButton);
    AddObject(newgameButtonObj);

    GameObject *loadButtonObj = new GameObject();
    this->loadButton = new Button(*loadButtonObj,{967,675},"assets/img/buttons/normal/loadButton.png","assets/img/buttons/highlighted/loadButton.png","assets/img/buttons/selected/loadButton.png");
    loadButtonObj->AddComponent(loadButton);
    AddObject(loadButtonObj);

    GameObject *optionsButtonObj = new GameObject();
    this->optionsButton = new Button(*optionsButtonObj,{968,784},"assets/img/buttons/normal/optionsButton.png","assets/img/buttons/highlighted/optionsButton.png","assets/img/buttons/selected/optionsButton.png");
    optionsButtonObj->AddComponent(optionsButton);
    AddObject(optionsButtonObj);

    GameObject *quitButtonObj = new GameObject();
    this->quitButton = new Button(*quitButtonObj,{968,890},"assets/img/buttons/normal/quitButton.png","assets/img/buttons/highlighted/quitButton.png","assets/img/buttons/selected/quitButton.png");
    quitButtonObj->AddComponent(quitButton);
    AddObject(quitButtonObj);

    GameObject *infoButtonObj = new GameObject();
    this->infoButton = new Button(*infoButtonObj,{1734,966},"assets/img/buttons/normal/infoButton.png","assets/img/buttons/highlighted/infoButton.png","assets/img/buttons/selected/infoButton.png");
    infoButtonObj->AddComponent(infoButton);
    AddObject(infoButtonObj);

    GameData::GetListOfSaves("assets/saves/listofsaves.txt");
}

MenuState::~MenuState(){
    GameData::SaveListOfSaves("assets/saves/listofsaves.txt");
    objectArray.clear();
    delete windoweffects;
}

void MenuState::LoadAssets(){

}

void MenuState::Update(float dt){
    windoweffects->Update(dt);
	InputManager *input = &(InputManager::GetInstance());
    State::UpdateArray(dt);
    if(newgameButton->isSelected()){
        GameData::AddSave("newgameplus");
        GameData::ChooseSave(GameData::saveFiles.size() - 1);
        windoweffects->FadeToBlack(1);
        // PlaySoundEffect("assets/audio/effects/select.wav");
    }
    else if(loadButton->isSelected()){
        windoweffects->FadeToBlack(1);
        // PlaySoundEffect("assets/audio/effects/select.wav");
        PlaySoundEffect("assets/audio/effects/startgame.ogg");
    }
    else if(optionsButton->isSelected()){
        // PlaySoundEffect("assets/audio/effects/select.wav");
    }
    else if(infoButton->isSelected()){
        // PlaySoundEffect("assets/audio/effects/select.wav");
    }
    else if(quitButton->isSelected()){
        // PlaySoundEffect("assets/audio/effects/select.wav");
        quitRequested = true;
    }
    #ifdef DEBUG
    else if(input->KeyPress(SDLK_SPACE)){
        windoweffects->FadeToBlack(1);
        // PlaySoundEffect("assets/audio/effects/select.wav");
        PlaySoundEffect("assets/audio/effects/startgame.ogg");
    } 
    #endif

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

    if(windoweffects->IsBlack()){
        Game::GetInstance().Push(new StageState());
    }
}

void MenuState::PlaySoundEffect(std::string file,int times){
    GameObject *effectObj = new GameObject();
    Sound *sound = new Sound(*effectObj,file);
    sound->Play(times,true);
    effectObj->AddComponent(sound);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
}

void MenuState::Render(){
    State::RenderArray();
    windoweffects->Render();
}

void MenuState::Start(){

}

void MenuState::Resume(){
    Camera::pos.Transform(0,0);
    newgameButton->Reset();
    loadButton->Reset();
    optionsButton->Reset();
    quitButton->Reset();
    windoweffects->FadeFromBlack(1);
    Game::GetInstance().GetMusic()->Open("assets/audio/musics/belmenu.ogg");
    Game::GetInstance().GetMusic()->Play();
}

void MenuState::Pause(){
    Game::GetInstance().GetMusic()->Stop();
}