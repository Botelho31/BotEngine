#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Player.h"

Minion::Minion(GameObject& associated) : Component(associated){
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    gravspeed = 2000;
    hittheground = new Timer();

    hp = 150;

    idletimer = new Timer();
    idle = false;

    state = IDLE;
    this->physics = new Physics(&associated);

    Sprite *minion =  new Sprite(associated,"assets/img/miniontest.png");
    this->minionsprite = minion;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(minion);
    SetCollider(0.5,0.9);
}

Minion::~Minion(){
    physics = nullptr;
    minionsprite = nullptr;
    delete idletimer;
    delete hittheground;
}

void Minion::Start(){

}

void Minion::Update(float dt){
    Component *component = associated.GetComponent("Collider");
    Collider *collider = dynamic_cast<Collider*>(component);
    physics->Update(collider->box);
    physics->CorrectDistance();
    std::cout << physics->DistanceTo(GetPosition(),Player::player->GetPosition(),500) << std::endl;
    XMovement(dt);
    YMovement(dt);
    IdleHandle(dt);

    if(hp <= 0){
        associated.RequestDelete();
    }
}

void Minion::XMovement(float dt){
    //Perfoms Movement if Allowed
    if((physics->distright - (speed.x * dt)) < 0){
        associated.box.x += physics->distright;
        speed.x = 0;
    }else if((physics->distleft + (speed.x * dt)) < 0){
        associated.box.x -= physics->distleft;
        speed.x = 0;
    }else if((physics->distright < 0) && (speed.x > 0)){
        speed.x = 0;
    }else if((physics->distleft < 0) && (speed.x < 0)){
        speed.x = 0;
    }else{
        associated.box.x += speed.x * dt;
    }
}
void Minion::YMovement(float dt){


    //Performs movement if it is allowed
    if(((physics->distground - (speed.y * dt)) < 0) && (speed.y > 0)){
        associated.box.y += physics->distground;
    }
    else if((physics->distceiling + (speed.y * dt) < 0) && (speed.y < 0)){
        associated.box.y -= physics->distceiling;
        speed.y = 0;
    } 
    else{
        associated.box.y += speed.y * dt;
    }
    //GRAVITY
    if(physics->distground > 0){
        if(idle == true){
            idletimer->Restart();
            idle = false;
        }
        speed.y += gravspeed*dt;
    }
}
void Minion::IdleHandle(float dt){
    if((idle == false) && (speed.x == 0) && (speed.y == 0)){
        idletimer->Update(dt);
        if((idletimer->Get() > 2) && (idle == false)){
            idle = true;
        }
    }else{
        idletimer->Restart();
    }
}

void Minion::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    minionsprite->SetFrameCount(framecount);
    minionsprite->SetFrameTime(frametime);
    minionsprite->SetRepeat(repeat);
    minionsprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
}

void Minion::SetCollider(float scaleX,float scaleY,float offsetX,float offsetY){
    Component *component = associated.GetComponent("Collider");
    Collider *collider = dynamic_cast<Collider*>(component);
    collider->SetScale(Vec2(scaleX,scaleY));
    collider->SetOffSet(Vec2(offsetX,offsetY));
}

void Minion::Render(){

}

bool Minion::Is(std::string type){
    if(type == "Minion"){
        return true;
    }else{
        return false;
    }
}

void Minion::NotifyCollision(GameObject& other){

}

Vec2 Minion::GetPosition(){
    return Vec2(associated.box.x + associated.box.w/2,associated.box.y + associated.box.h/2);
}
