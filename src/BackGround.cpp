#include "../include/BackGround.h"
#include "../include/Camera.h"

BackGround::BackGround(GameObject& go,std::string backgroundfile) : Component(go){
    originalpos = Vec2(go.box.x,go.box.y);
    this->backgroundsprite = new Sprite(associated,backgroundfile);
    associated.AddComponent(backgroundsprite);
}

void BackGround::Update(float dt){
    associated.box.x = originalpos.x - Camera::pos.x * 0.25;
    associated.box.y = originalpos.y;
}

void BackGround::Render(){

}

void BackGround::SetFile(std::string file){
    backgroundsprite->Open(file);
}

bool BackGround::Is(std::string type){
    if(type == "BackGround"){
        return true;
    }else{
        return false;
    }
}