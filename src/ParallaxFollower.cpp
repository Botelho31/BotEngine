#include "../include/ParallaxFollower.h"
#include "../include/Camera.h"

ParallaxFollower::ParallaxFollower(GameObject& go,float parallax) : Component(go){
    originalpos = Vec2(go.box.x,go.box.y);
    this->parallax = parallax;
    this->catchParallax = true;
    this->speed = Vec2(600,600);
}

ParallaxFollower::~ParallaxFollower(){
}

void ParallaxFollower::Update(float dt){
    if(parallax != 1){
        if(catchParallax){
            if(Follow({originalpos.x + Camera::pos.x * parallax,associated.box.GetCenter().y},300,dt)){
                catchParallax = true;
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

bool ParallaxFollower::Follow(Vec2 dest,float constspeed,float dt){
    Vec2 speed;
    // float angle = associated.box.GetCenter().GetAngle(dest.x,dest.y);
    // speed.x = abs(constspeed * cos(angle));
    // speed.y = abs(constspeed * sin(angle));
    speed.x =  constspeed;
    speed.y = constspeed;

    bool gotx = false;
    bool goty = false;
    if(associated.box.x == dest.x){
        gotx = true;
    }
    else if(associated.box.x < dest.x){
        associated.box.x += speed.x * dt;
        if(associated.box.x > dest.x){
            associated.box.x = dest.x;
        }
    }else{
        associated.box.x -= speed.x * dt;
        if(associated.box.x < dest.x){
            associated.box.x = dest.x;
        }
    }


    if(associated.box.GetCenter().y == dest.y){
        goty = true;
    }
    else if(associated.box.GetCenter().y < dest.y){
        associated.box.y += speed.y * dt;
        if(associated.box.GetCenter().y > dest.y){
            associated.box.y = dest.y;
        }
    }else{
        associated.box.y -= speed.y * dt;
        if(associated.box.GetCenter().y < dest.y){
            associated.box.y = dest.y;
        }
    }
    
    if(gotx && goty){
        return true;
    }else{
        return false;
    }
}

void ParallaxFollower::AddOriginalPos(Vec2 add){
    originalpos.x += add.x;
    originalpos.y += add.y;
}

void ParallaxFollower::SetParallax(float parallax,bool catchParallax){
    this->parallax = parallax;
    this->catchParallax = catchParallax;
}