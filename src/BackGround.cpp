#include "../include/BackGround.h"
#include "../include/Camera.h"
#include "../include/CameraFollower.h"

BackGround::BackGround(GameObject& go,std::string backgroundfile,bool parallax,bool camerafollower) : Component(go){
    originalpos = Vec2(go.box.x,go.box.y);
    this->backgroundsprite = new Sprite(associated,backgroundfile);
    associated.AddComponent(backgroundsprite);
    this->parallax = parallax;
    this->camerafollower = camerafollower;
}

void BackGround::Update(float dt){
    if(parallax){
        associated.box.x = originalpos.x + Camera::pos.x * 0.5;
        associated.box.y = originalpos.y;
    }else{
        if(camerafollower){
            associated.box.x = Camera::pos.x;
            associated.box.y = Camera::pos.y;
        }else{
            associated.box.x = originalpos.x + Camera::pos.x;
            associated.box.y = originalpos.y + Camera::pos.y;
        }
    }
}

void BackGround::Render(){

}

void BackGround::ChangeBackground(std::string file,bool parallax,Vec2 scale,bool camerafollower){
    backgroundsprite->Open(file);
    backgroundsprite->SetScaleX(scale.x,scale.y);
    this->parallax = parallax;
    this->camerafollower = camerafollower;
}

bool BackGround::Is(std::string type){
    if(type == "BackGround"){
        return true;
    }else{
        return false;
    }
}