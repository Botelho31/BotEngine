#include "../include/Soul.h"

Soul::Soul(GameObject& associated,int soulID) : Component(associated){
    this->soulID = soulID;
    catched = false;
    normalsprite = "assets/img/beltransparent.png";
    catchedsprite = "assets/img/beltransparent.png";
    catchinganimation =  new Timer();

    this->soulsprite = new Sprite(associated,normalsprite);
    associated.AddComponent(soulsprite);

    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

Soul::~Soul(){
    delete catchinganimation;
}

void Soul::Update(float dt){
    if(catchinganimation->Started()){
        catchinganimation->Update(0);
        if(catchinganimation->Get() > 2){
            catchinganimation->Restart();
            associated.RequestDelete();
        }
    }
}

void Soul::Render(){

}

bool Soul::Is(std::string type){
    if(type == "Soul"){
        return true;
    }else{
        return false;
    }
}

void Soul::NotifyCollision(GameObject& other){
    if(!catched){
        Component *component = other.GetComponent("Player");
        if(component){
            catched = true;
            catchinganimation->Delay(0);
            SetSprite(catchedsprite);

            //LOGIC THAT WORKS AFTER PLAYER CATCHES SOUL

        }
    }
}


void Soul::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    soulsprite->SetFrameCount(framecount);
    soulsprite->SetFrameTime(frametime);
    soulsprite->SetRepeat(repeat);
    soulsprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
}