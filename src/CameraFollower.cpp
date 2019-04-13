#include "../include/CameraFollower.h"
#include "../include/Camera.h"

CameraFollower::CameraFollower(GameObject& go) : Component(go){
    originalpos = Vec2(go.box.x,go.box.y);
}

void CameraFollower::Update(float dt){
    associated.box.x = originalpos.x + Camera::pos.x;
    associated.box.y = originalpos.y + Camera::pos.y;
}

void CameraFollower::Render(){

}

bool CameraFollower::Is(std::string type){
    if(type == "CameraFollower"){
        return true;
    }else{
        return false;
    }
}