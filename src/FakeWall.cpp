#include "../include/FakeWall.h"
#include "../include/Collider.h"
#include "../include/HitBox.h"
#include "../include/TileMap.h"

FakeWall::FakeWall(GameObject& associated,std::string wallspritefile,bool breakable,int hp) : Component(associated){
    wallsprite = new Sprite(associated,wallspritefile);
    associated.AddComponent(wallsprite);
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    this->hp = hp;
    this->alpha = 255;
    this->alphapersecond = 200;
    this->currentEffect = STANDBY;
    this->breakable = breakable;
    this->invincibilitytimer = new Timer();
    if(breakable){
        TileCollider* tilecollider = new TileCollider(associated,associated.box,true);
        std::weak_ptr<Component> weakptr = associated.AddComponent(tilecollider);
        TileMap::tiles.push_back(weakptr);
    }
}

FakeWall::~FakeWall(){
    wallsprite = nullptr;
    delete invincibilitytimer;
}

void FakeWall::Update(float dt){
    if(breakable){
        if(invincibilitytimer->Started()){
            invincibilitytimer->Update(dt);
            if(invincibilitytimer->Get() > 0.5){
                invincibilitytimer->Restart();
            }
        }
        if(hp <= 0){
            associated.RequestDelete();
        }
    }else{
        switch(currentEffect){
            case FADE:
                if((alpha - (alphapersecond *dt)) <= 100){
                    alpha = 100;
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
    if(breakable && !invincibilitytimer->Started()){
        if(other.GetComponent("HitBox")){
            HitBox *hitbox = dynamic_cast<HitBox*>(other.GetComponent("HitBox"));
            if(hitbox->HitEnemy()){
                hp -= hitbox->GetDamage();
                invincibilitytimer->Delay(0);
            }
        }
    }else{
        if(other.GetComponent("Player")){
            currentEffect = FADE;
        }
    }
}