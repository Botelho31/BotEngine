#include "../include/FakeWall.h"
#include "../include/Collider.h"
#include "../include/HitBox.h"

FakeWall::FakeWall(GameObject& associated,std::string wallspritefile,bool breakable,int hp) : Component(associated){
    wallsprite = new Sprite(associated,wallspritefile);
    associated.AddComponent(wallsprite);
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    this->hp = hp;
    this->alpha = 255;
    this->alphapersecond = 200;
    this->currentEffect = STANDBY;
    if(breakable){
        TileCollider* tilecollider = new TileCollider(associated,associated.box,true);
        associated.AddComponent(tilecollider);
    }
}

FakeWall::~FakeWall(){
    wallsprite = nullptr;
}

void FakeWall::Update(float dt){
    if(breakable){
        if(hp <= 0){
            associated.RequestDelete();
        }
    }else{
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
    if(breakable){
        if(other.GetComponent("HitBox")){
            HitBox *hitbox = dynamic_cast<HitBox*>(other.GetComponent("HitBox"));
            if(hitbox->HitEnemy()){
                hp -= hitbox->GetDamage();
            }
        }
    }else{
        if(other.GetComponent("Player")){
            currentEffect = FADE;
        }
    }
}