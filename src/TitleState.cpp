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
#include "../include/GameData.h"
#include "../include/Sound.h"
#include "../include/Rotating.h"

TitleState::TitleState(){
    quitRequested = false;
    popRequested = false;

    GameData::PreLoadMusics("assets/audio/info/musics.txt");
    GameData::PreLoadSounds("assets/audio/info/effects.txt");

    Game::GetInstance().GetMusic()->Open("assets/audio/musics/belmenu.ogg");
    Game::GetInstance().GetMusic()->Play();

    GameObject *titleobj =  new GameObject();
    Sprite *title = new Sprite(*titleobj,"assets/img/HUD/startscreen.jpg");
    titleobj->box.x = 0;
    titleobj->box.y = 0;
    titleobj->AddComponent(title);
    AddObject(titleobj);

    //START CIRCLES
    GameObject *circleobj =  new GameObject();
    Sprite *circle = new Sprite(*circleobj,"assets/img/HUD/circlestart.png");
    circleobj->box.x = -400;
    circleobj->box.y = 650;
    Rotating *rotating1 = new Rotating(*circleobj,-1);
    circleobj->AddComponent(rotating1);
    circleobj->AddComponent(circle);
    AddObject(circleobj);

    GameObject *circle2obj =  new GameObject();
    Sprite *circle2 = new Sprite(*circle2obj,"assets/img/HUD/circle2start.png");
    circle2obj->box.x = 1362;
    circle2obj->box.y = 250;
    Rotating *rotating2 = new Rotating(*circle2obj,1);
    circleobj->AddComponent(rotating2);
    circle2obj->AddComponent(circle2);
    AddObject(circle2obj);

    GameObject *circle3obj =  new GameObject();
    Sprite *circle3 = new Sprite(*circle3obj,"assets/img/HUD/circle3start.png");
    circle3obj->box.x = 520;
    circle3obj->box.y = -350;
    Rotating *rotating3 = new Rotating(*circle3obj,1);
    circle3obj->AddComponent(rotating3);
    circle3obj->AddComponent(circle3);
    AddObject(circle3obj);

    GameObject *rebellioobj =  new GameObject();
    Sprite *rebellio = new Sprite(*rebellioobj,"assets/img/HUD/rebelliostart.png");
    rebellioobj->box.x = 212;
    rebellioobj->box.y = -40;
    rebellioobj->AddComponent(rebellio);
    AddObject(rebellioobj);

    //END CIRCLES

    GameObject *mousePointerObj = new GameObject();
    MousePointer *mousepointer = new MousePointer(*mousePointerObj);
    mousePointerObj->AddComponent(mousepointer);
    AddObject(mousePointerObj);

    GameObject *textObj = new GameObject();
    Sprite* text = new Sprite(*textObj,"assets/img/HUD/titlePagetext.png");
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

    if(input->IsKeyDown(SDLK_5)){
        std::cout << input->GetMouseX() << " " << input->GetMouseY() << std::endl;
    }
    if(playButton->isSelected()){
        PlaySoundEffect("assets/audio/effects/select.ogg");
        Game::GetInstance().Push(new MenuState());
    }
    else if(input->KeyPress(SDLK_SPACE)){
        PlaySoundEffect("assets/audio/effects/select.ogg");
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

void TitleState::PlaySoundEffect(std::string file,int times){
    GameObject *effectObj = new GameObject();
    Sound *sound = new Sound(*effectObj,file);
    sound->Play(times,true);
    effectObj->AddComponent(sound);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
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