#include "../include/State.h"
#include "../include/Face.h"
#include "../include/Vec2.h"
#include "../include/Sound.h"
#include "../include/TileSet.h"
#include "../include/TileMap.h"
#include "../include/InputManager.h"

State::State(Resources* resources) : resources(resources){
    quitRequested = false;
    music = new Music(resources,"assets/audio/stageState.ogg");
    music->Play();

	GameObject *background = new GameObject();
    bg = new Sprite(*background,resources,"assets/img/ocean.jpg");
	background->AddComponent(bg);
	objectArray.emplace_back(background);

	GameObject *tileObj = new GameObject();
	TileSet *tileSet = new TileSet(64,64,resources,"assets/img/tileset.png");
	TileMap *tileMap = new TileMap(*tileObj,"assets/map/tileMap.txt",tileSet);
	tileObj->box.x = 0;
	tileObj->box.y = 0;
	tileObj->AddComponent(tileMap);
	objectArray.emplace_back(tileObj);

}

State::~State(){
	if(music != nullptr){
		delete music;
	}
	std::cout << "Cleared "<< objectArray.size() << " Objects" << std::endl;
    objectArray.clear();
}

void State::AddObject(int mouseX,int mouseY){
    GameObject *object = new GameObject();
    Sprite *penguin  =  new Sprite(*object,resources,"assets/img/penguinface.png");
    object->AddComponent(penguin);
    object->box.x = mouseX - (object->box.w/2);
    object->box.y = mouseY - (object->box.h/2);
    Sound *sound = new Sound(*object,resources,"assets/audio/boom.wav");
    object->AddComponent(sound);
    Face *face = new Face(*object);
    object->AddComponent(face);
    objectArray.emplace_back(object);
}

void State::LoadAssets(){

}

void State::Update(float dt){
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_ESCAPE) || input->QuitRequested()){
		quitRequested = true;
	}
	if(input->IsKeyDown(SDLK_SPACE)){
		Vec2 objPos = Vec2( 200, 0 ).GetRotated( -PI + PI*(rand() % 1001)/500.0 ) + Vec2( input->GetMouseX(), input->GetMouseY() );
		AddObject((int)objPos.x, (int)objPos.y);
	}
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Update(dt);
    }
    for(unsigned int i = 0; i < objectArray.size();i++){
        if(objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin() + i);
        }
    }
}

void State::Render(){
    for(unsigned int i = 0; i < objectArray.size();i++){
        objectArray[i]->Render();
    }
}

bool State::QuitRequested(){
    return quitRequested;
}