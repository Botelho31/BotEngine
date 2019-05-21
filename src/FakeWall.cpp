#include "../include/FakeWall.h"
#include "../include/Collider.h"

FakeWall::FakeWall(GameObject& associated,std::string wallspritefile) : Component(associated){
    wallsprite = new Sprite(associated,wallspritefile);
    associated.AddComponent(wallsprite);
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);

    alpha = 255;
    alphapersecond = 200;
    currentEffect = STANDBY;
}

FakeWall::~FakeWall(){
    wallsprite = nullptr;
}

void FakeWall::Update(float dt){
    switch(currentEffect){
        case FADE:
            if((alpha - (alphapersecond *dt)) <= 122){
                alpha = 122;
            }else{
                alpha -= alphapersecond * dt;
            }
            wallsprite->SetAlpha((int)alpha);
            currentEffect = UNFADE;
            break;
        case UNFADE:
            if((alpha + (alphapersecond *dt)) >= 255){
                alpha = 255;
                currentEffect = STANDBY;
                break;
            }else{
                alpha += alphapersecond * dt;
            }
            wallsprite->SetAlpha((int)alpha);
            break;
        case STANDBY:
            break;
        default:
            break;
    }
}

void FakeWall::Render(){

}

bool FakeWall::Is(std::string type){
    if(type == "FakeWall"){
        return true;
    }else{
        return false;
    }
}

void FakeWall::NotifyCollision(GameObject& other){
    if(other.GetComponent("Player")){
        currentEffect = FADE;
    }
}