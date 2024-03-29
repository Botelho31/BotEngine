#include "../include/Soul.h"
#include "../include/GameData.h"
#include "../include/Sound.h"
#include "../include/Player.h"

Soul::Soul(GameObject& associated,int soulID) : Component(associated){
    this->soulID = soulID;
    catched = false;
    normalsprite = "assets/img/soul.png";
    catchedsprite = "assets/img/souldissapear.png";
    catchinganimation =  new Timer();

    this->soulsprite = new Sprite(associated,normalsprite,48,0.04);
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
        if(catchinganimation->Get() > 1){
            catchinganimation->Restart();
            if(GameData::listOfDiscoveredSouls.size() == 3){
                GameData::playerSword = true; //WHERE PLAYER GETS SWORD
                GameObject* swordeventObj = new GameObject();
                Event *swordevent = new Event(*swordeventObj,Event::PLAYERGETSSWORD,0.5);
                swordeventObj->AddComponent(swordevent);
                GameData::events.push(swordevent);
                if(Player::player){
                    std::vector<std::string> spritefile;
                    spritefile.push_back("assets/img/info/playersword.txt");
                    spritefile.push_back("assets/img/info/effects.txt");
                    Player::player->LoadSpriteFiles(spritefile);
                }
            }
            if(GameData::listOfDiscoveredSouls.size() == 6){
                GameData::playerDoubleJump = true; //WHERE PLAYER GETS DoubleJump
                GameObject* jumpeventObj = new GameObject();
                Event *jumpevent = new Event(*jumpeventObj,Event::PLAYERGETSDOUBLEJUMP,0.5);
                jumpeventObj->AddComponent(jumpevent);
                GameData::events.push(jumpevent);
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
            PlaySoundEffect("assets/audio/effects/almasound.ogg");
            std::cout << "Catched soul" << std::endl;
            catched = true;
            catchinganimation->Delay(0);
            SetSprite(catchedsprite,13,0.04,false);
            //LOGIC THAT WORKS AFTER PLAYER CATCHES SOUL
            GameData::listOfDiscoveredSouls.push_back(soulID);
            GameData::checkpointPos = associated.box.GetCenter();
            GameData::checkpointPosSpeed = Vec2(0,0);
        }
    }
}

void Soul::PlaySoundEffect(std::string file,int times){
    GameObject *effectObj = new GameObject();
    Sound *sound = new Sound(*effectObj,file);
    sound->Play(times,true);
    effectObj->AddComponent(sound);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
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