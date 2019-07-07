#include "../include/Soul.h"
#include "../include/GameData.h"
#include "../include/Sound.h"

Soul::Soul(GameObject& associated,int soulID) : Component(associated){
    this->soulID = soulID;
    catched = false;
    normalsprite = "assets/img/testsoul.png";
    catchedsprite = "assets/img/testsoul.png";
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
        catchinganimation->Update(dt);
        if(catchinganimation->Get() > 14){
            catchinganimation->Restart();
            if(GameData::listOfDiscoveredSouls.size() == 3){
                GameData::playerSword = true; //WHERE PLAYER GETS SWORD
                GameObject* swordeventObj = new GameObject();
                Event *swordevent = new Event(*swordeventObj,Event::PLAYERGETSSWORD,2);
                swordeventObj->AddComponent(swordevent);
                GameData::events.push(swordevent);
            }
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
            PlaySound("assets/audio/effects/almasound.ogg",1);
            std::cout << "Catched soul" << std::endl;
            catched = true;
            catchinganimation->Delay(0);
            SetSprite(catchedsprite);
            //LOGIC THAT WORKS AFTER PLAYER CATCHES SOUL
            GameData::listOfDiscoveredSouls.push_back(soulID);
            GameData::checkpointPos = associated.box.GetCenter();
            GameData::checkpointPosSpeed = Vec2(0,0);
        }
    }
}

void Soul::PlaySound(std::string file,int times){
    Component *soundcomp = associated.GetComponent("Sound");
    if(soundcomp){
        Sound *sound = dynamic_cast<Sound*>(soundcomp);
        sound->Open(file);
        sound->Play(times);
    }else{
        Sound *sound = new Sound(associated,file);
        sound->Play(times);
        associated.AddComponent(sound);
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