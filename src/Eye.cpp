#include "../include/Eye.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"
#include "../include/Player.h"
#include "../include/Camera.h"
#include "../include/Minion.h"
#include "../include/ParallaxFollower.h"
#include "../include/GameData.h"
#include "../include/Sound.h"

Eye::Eye(GameObject& associated,Circle bounds,Vec2 end,int pupilradius,bool keepPupilIn) : 
    Component(associated){
    pupil.radius = pupilradius;
    this->bounds = bounds;
    this->keepPupilIn = keepPupilIn;
    std::vector<std::string> spritefile;
    spritefile.push_back("assets/img/info/eye.txt");
    spritefile.push_back("assets/img/info/effects.txt");
    this->spritefiles = GameData::GetSpritesFiles(spritefile);
    std::vector<std::string> soundfile;
    soundfile.push_back("assets/audio/info/effects.txt");
    this->soundfiles = GameData::GetSpritesFiles(soundfile);
    this->eyelid = new Sprite(associated,spritefiles["transparent"]);
    associated.AddComponent(eyelid);
    associated.box.SetCenter({bounds.x,bounds.y});
    this->pupil.Transform(associated.box.GetCenter());
    start = associated.box.GetCenter();
    this->end = end;
    stopPrint = false;

    eyepop = new Timer();
    eyein = new Timer();
    eyedelay = new Timer();

}

Eye::~Eye(){
    delete eyepop;
    delete eyein;
    delete eyedelay;
}

void Eye::Start(){
    associated.ChangeComponentOrder("Eye","Sprite");
}

void Eye::Update(float dt){
    Vec2 offset = associated.box.GetCenter().Added(- Camera::pos.x,-Camera::pos.y);

    Vec2 boundsbefore  = bounds.GetCenter();
    bounds.Transform(offset);
    Vec2 boundsafter = bounds.GetCenter();
    pupil.x += boundsafter.x - boundsbefore.x;
    pupil.y += boundsafter.y - boundsbefore.y;

    if(eyepop->Started() || eyein->Started() || eyedelay->Started()){
        if(stopPrint){
            if(eyepop->Started()){
                eyepop->Update(dt);
                if(eyepop->Get() > 1.28){
                    SetSprite(spritefiles["eyeempty"]);
                    PlaySoundEffect(soundfiles["pop"]);
    
                    GameObject *minionObj =  new GameObject();
                    Minion *minion = new Minion(*minionObj,Minion::FALLINGFROMBOSS);
                    minionObj->box.SetCenter(associated.box.GetCenter());
                    minionObj->AddComponent(minion);
                    Game::GetInstance().GetCurrentState().AddObject(minionObj);

                    eyepop->Restart();

                    eyedelay->Delay(0);

                }
            }
            else if(eyedelay->Started()){
                eyedelay->Update(dt);
                if(eyedelay->Get() > 3){
                    SetSprite(spritefiles["eyein"],32,0.04,false);
                    eyedelay->Restart();
                    eyein->Delay(0);
                }
            }
            else if(eyein->Started()){
                eyein->Update(dt);
                if(eyein->Get() > 1.28){
                    eyein->Restart();
                    SetSprite(spritefiles["transparent"]);
                    stopPrint = false;
                }
            }
        }else if(PupilFollow(bounds.GetCenter(),200,dt) == Vec2(0,0)){
            stopPrint = true;
            if(eyepop->Started()){
                SetSprite(spritefiles["eyepop"],34,0.04,false);
            }
        }
    }else{
        PupilFollow(Player::player->GetPosition().Added(- Camera::pos.x, - Camera::pos.y),200,dt);

        float adjustdist = bounds.radius - pupil.GetDistanceFromCenter(bounds.x,bounds.y);
        float angle = bounds.GetAngleFromCenter(pupil.x,pupil.y);
        if(keepPupilIn){
            adjustdist = fabs(bounds.radius - pupil.radius) - pupil.GetDistanceFromCenter(bounds.x,bounds.y);
        }
        if((!bounds.IsInside(pupil) && keepPupilIn) || (!bounds.IsInside(pupil.GetCenter()) && !keepPupilIn)){
            if(pupil.x < bounds.x){
                pupil.x += std::fabs(adjustdist * cos(angle));
            }else{
                pupil.x -= std::fabs(adjustdist * cos(angle));
            }
            if(pupil.y < bounds.y){
                pupil.y += std::fabs(adjustdist * sin(angle));
            }else{
                pupil.y -= std::fabs(adjustdist * sin(angle));
            }
        }
    }
}

bool Eye::GoToEndPoint(float constspeed,float dt){
    return Follow(end,constspeed,dt);
}

bool Eye::GoToStartPoint(float constspeed,float dt){
    return Follow(start,constspeed,dt);
}

bool Eye::Follow(Vec2 dest,float constspeed,float dt){
    Vec2 speed;
    float angle = associated.box.GetCenter().GetAngle(dest.x,dest.y);
    speed.x = abs(constspeed * cos(angle));
    speed.y = abs(constspeed * sin(angle));

    bool gotx = false;
    bool goty = false;
    if(associated.box.GetCenter().x == dest.x){
        gotx = true;
    }
    else if(associated.box.GetCenter().x < dest.x){
        associated.box.x += speed.x * dt;
        if(associated.box.GetCenter().x > dest.x){
            associated.box.x = dest.x;
        }
    }else{
        associated.box.x -= speed.x * dt;
        if(associated.box.GetCenter().x < dest.x){
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
    }
}


Vec2 Eye::PupilFollow(Vec2 dest,float constspeed,float dt){
    Vec2 delta;
    float angle = pupil.GetAngleFromCenter(dest.x,dest.y);
    speed.x = abs(constspeed * cos(angle));
    speed.y = abs(constspeed * sin(angle));
    if(pupil.x == dest.x){
        pupil.x = dest.x;
        delta.x = 0;
    }
    else if(pupil.x < dest.x){
        pupil.x += speed.x * dt;
        delta.x = speed.x * dt;
        if(pupil.x > dest.x){
            pupil.x = dest.x;
            delta.x = 0;
        }
    }else{
        pupil.x -= speed.x * dt;
        delta.x = -(speed.x * dt);
        if(pupil.x < dest.x){
            pupil.x = dest.x;
            delta.x = 0;
        }
    }


    if(pupil.y == dest.y){
        pupil.y = dest.y;
        delta.y = 0;
    }
    else if(pupil.y < dest.y){
        pupil.y += speed.y * dt;
        delta.y = speed.y * dt;
        if(pupil.y > dest.y){
            pupil.y = dest.y;
            delta.y = 0;
        }
    }else{
        pupil.y -= speed.y * dt;
        delta.y = -(speed.y * dt);
        if(pupil.y < dest.y){
            pupil.y = dest.y;
            delta.y = 0;
        }
    }
    return delta;
}

void Eye::Render() { // .Added(-Camera::pos.x,- Camera::pos.y)
    if(!stopPrint){
        if(!keepPupilIn){
            WindowEffects::FillCircleIfInside(pupil,bounds);
        }else{
            WindowEffects::FillCircle(pupil,0,0,0,255);
        }
    }

    #ifdef DEBUG
        InputManager *input = &(InputManager::GetInstance());
        if(input->IsKeyDown(SDLK_EQUALS)){
            WindowEffects::DrawCircle(bounds,0,0,0,255);
        }
    #endif 
    // DEBUG
    // eyelid->Render(bounds.x - associated.box.w/2,bounds.y - associated.box.h/2);
}

void Eye::NotifyCollision(GameObject& other){

}

bool Eye::Is(std::string type){
    if(type == "Eye"){
        return true;
    }else{
        return false;
    }
}

void Eye::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    eyelid->SetFrameCount(framecount);
    eyelid->SetFrameTime(frametime);
    eyelid->SetRepeat(repeat);
    eyelid->Open(file);
    Vec2 newbox = associated.box.GetCenter();
    associated.box.x = prepos.GetCenter().x - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.GetCenter().y - (associated.box.h/2) + offset.y;
    Vec2 adjustedbox = associated.box.GetCenter();
    Component *comp = associated.GetComponent("ParallaxFollower");
    if(comp){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
        parallaxfollower->AddOriginalPos({adjustedbox.x - newbox.x,adjustedbox.y - newbox.y});
    }
}

void Eye::PlaySoundEffect(std::string file,int times){
    GameObject *effectObj = new GameObject();
    Sound *sound = new Sound(*effectObj,file);
    sound->Play(times,true);
    effectObj->AddComponent(sound);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
}

void Eye::SpawnMinion(){
    if(!stopPrint){
        eyepop->Delay(0);
    }
}