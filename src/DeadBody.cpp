#include "../include/DeadBody.h"
#include "../include/HitBox.h"


DeadBody::DeadBody(GameObject& associated,Vec2 dyingspeed,Sprite *dyingsprite,Vec2 colliderscale,Vec2 collideroffset,bool interaction) : Component(associated){
    speed = dyingspeed;
    despeed = 800;
    gravspeed = 2000;

    invincibilitytimer = new Timer();
    damageCooldown = 0;

    idletimer = new Timer();
    idle = false;

    this->physics = new Physics(associated,&speed,false);
    associated.AddComponent(physics);
    this->interaction = interaction;

    this->deadbodysprite =  dyingsprite;
    associated.AddComponent(deadbodysprite);
    physics->SetCollider(colliderscale.x,colliderscale.y,collideroffset.x,collideroffset.y);
}

DeadBody::~DeadBody(){
    deadbodysprite = nullptr;
    physics = nullptr;
    delete idletimer;
    delete invincibilitytimer;
}

void DeadBody::Start(){

}

void DeadBody::Update(float dt){
    if(!idle){
        // physics->Update(dt);
        physics->PerformXDeceleration(despeed,dt);
        physics->PerformXMovement(dt);
        physics->PerformYMovement(dt);
        physics->PerformGravity(gravspeed,dt);
        IdleHandle(dt);
    }
    if(idle && !interaction){
        // associated.RemoveComponent(associated.GetComponent("Physics"));
        // associated.RemoveComponent(associated.GetComponent("Collider"));
        // physics = nullptr;
    }
    if(interaction){
        if(invincibilitytimer->Started()){
            invincibilitytimer->Update(dt);
            if(invincibilitytimer->Get() >= damageCooldown){
                invincibilitytimer->Restart();
                this->damageCooldown = 0;
            }
        }
    }
}

void DeadBody::Render(){

}

bool DeadBody::Is(std::string type){
    if(type == "DeadBody"){
        return true;
    }else{
        return false;
    }
}

void DeadBody::NotifyCollision(GameObject& other){
    if(!invincibilitytimer->Started() && interaction){
        Component *component1 = other.GetComponent("HitBox");
        if(component1){
            HitBox *hitbox = dynamic_cast<HitBox*>(component1);
            if((hitbox)  && hitbox->HitEnemy()){
                physics->KnockBack(hitbox->GetOwner()->box,hitbox->GetKnockBack());
                damageCooldown = hitbox->GetDamageCooldown();
                invincibilitytimer->Delay(0);
            }
        }
    }
}

void DeadBody::IdleHandle(float dt){
    if((speed.x == 0) && (speed.y == 0)){
        idletimer->Update(dt);
        if((idletimer->Get() > 2) && (idle == false)){
            idle = true;
        }
    }else{
        idle = false;
        idletimer->Restart();
    }
}

void DeadBody::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    deadbodysprite->SetFrameCount(framecount);
    deadbodysprite->SetFrameTime(frametime);
    deadbodysprite->SetRepeat(repeat);
    deadbodysprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
}

Vec2 DeadBody::GetPosition(){
    return physics->GetCollider()->box.GetCenter();
}