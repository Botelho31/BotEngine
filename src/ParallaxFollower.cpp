#include "../include/ParallaxFollower.h"
#include "../include/Camera.h"

ParallaxFollower::ParallaxFollower(GameObject& go,float parallax) : Component(go){
    originalpos = Vec2(go.box.x,go.box.y);
    this->parallax = parallax;
    this->catchParallax = false;
    this->speed = Vec2(600,600);
    this->physics = new Physics(associated,&speed,true);
}

ParallaxFollower::~ParallaxFollower(){
    delete physics;
}

void ParallaxFollower::Update(float dt){
    if(parallax != 1){
        if(catchParallax){
            physics->PrintValues();
            if(physics->Follow({(originalpos.x + associated.box.w/2) + Camera::pos.x * parallax,associated.box.GetCenter().y},600,dt) == Vec2(0,0)){
                catchParallax = false;
            }
        }else{
            associated.box.x = originalpos.x + Camera::pos.x * parallax;
        }
    }else{
        catchParallax = false;
    }
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

void ParallaxFollower::SetParallax(float parallax,bool catchParallax){
    this->parallax = parallax;
    this->catchParallax = catchParallax;
}