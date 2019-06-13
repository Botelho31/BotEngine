#include "../include/ParallaxFollower.h"
#include "../include/Camera.h"

ParallaxFollower::ParallaxFollower(GameObject& go,float parallax) : Component(go){
    originalpos = Vec2(go.box.x,go.box.y);
    this->parallax = parallax;
}

void ParallaxFollower::Update(float dt){
    associated.box.x = originalpos.x + Camera::pos.x * parallax;
}

void ParallaxFollower::Render(){

}

bool ParallaxFollower::Is(std::string type){
    if(type == "ParallaxFollower"){
        return true;
    }else{
        return false;
    }
}