#include "../include/Alien.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Resources.h"
#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Bullet.h"
#include "../include/Sound.h"
#include "../include/Timer.h"
#include "../include/PenguinBody.h"
#include "../include/Sprite.h"

int Alien::alienCount;

Alien::Alien(GameObject& associated,int nMinions) : Component(associated){
    alienCount ++;
    state = RESTING;
    restTimer = new Timer();
    restTimer->Restart();
    speed.x = 150;
    speed.y = 150;
    hp = 30;
    this->nMinions = nMinions;
    Sprite *alien = new Sprite(associated,"assets/img/alien.png");
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(alien);
}

Alien::~Alien(){
    alienCount --;
    delete restTimer;
}

void Alien::Start(){
    for(int i = 0; i < nMinions;i ++){
        GameObject *minionObj = new GameObject();
        std::weak_ptr<GameObject> alienCenter = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
        Minion *minion = new Minion(*minionObj,alienCenter,i*(PI*2/(float)nMinions));
        Component *component = minionObj->GetComponent("Sprite");
        if(component != nullptr){
            Sprite *sprite = dynamic_cast<Sprite*>(component);
            float scale = (rand() % 5) + 10;
            scale /= 10;
            sprite->SetScaleX(scale,scale);
        }
        minionObj->AddComponent(minion);
        std::weak_ptr<GameObject> minionWeakPtr = Game::GetInstance().GetCurrentState().AddObject(minionObj);
        minionArray.emplace_back(minionWeakPtr);

    }
}

void Alien::Update(float dt){
    associated.angleDeg -= (180/10) * dt;
    for(int i = 0;i < minionArray.size();i++){
        if(minionArray[i].use_count() == 0){
            minionArray.erase(minionArray.begin() + i);
        }
    }
    if(state == RESTING){
        restTimer->Update(dt);
        if(restTimer->Get() >= 5){
            if(PenguinBody::player != nullptr){
                state = MOVING;
                Vec2 position = PenguinBody::player->GetPosition();
                destination.Transform(position.x - associated.box.w/2,position.y - associated.box.h/2);
            }
        }
    }
    else if(state == MOVING){
        float x = destination.x;
        float y = destination.y;
        if(associated.box.Follow(x,y,speed.x,speed.y,dt)){ 
            state = RESTING;
            restTimer->Restart();
            Vec2 position = PenguinBody::player->GetPosition();
            destination.Transform(position.x - associated.box.w/2,position.y - associated.box.h/2);
            float lastDistance = 10000;
            int chosen = 0;
            for(int i = 0; i < minionArray.size();i++){
                float distance = minionArray[i].lock().get()->box.GetDistance(destination.x,destination.y);
                if(distance < lastDistance){
                    lastDistance = distance;
                    chosen = i;
                }
            }
            if(!minionArray.empty()){
                Component* component = minionArray[chosen].lock().get()->GetComponent("Minion");
                if(component){
                    Minion *minion = dynamic_cast<Minion*>(component);
                    minion->Shoot(destination);
                }
            }
        }
    }   

    if(hp <= 0){
        GameObject *explosionObj = new GameObject();
        Sprite *explosion = new Sprite(*explosionObj,"assets/img/aliendeath.png",4,0.2,0.8);
        explosionObj->box.Transform(associated.box.x + associated.box.w/2 - explosionObj->box.w/2,associated.box.y + associated.box.h/2 - explosionObj->box.h/2);
        explosionObj->angleDeg = associated.angleDeg;
        Sound *sound =  new Sound(*explosionObj,"assets/audio/boom.wav");
        sound->Play(1);
        explosionObj->AddComponent(explosion);
        explosionObj->AddComponent(sound);
        Game::GetInstance().GetCurrentState().AddObject(explosionObj);
        associated.RequestDelete();
    }

}

void Alien::Render(){

}

bool Alien::Is(std::string type){
    if(type == "Alien"){
        return true;
    }else{
        return false;
    }
}


void Alien::NotifyCollision(GameObject& other){
    Component *component = other.GetComponent("Bullet");    
    if(component){
        Bullet *bullet = dynamic_cast<Bullet*>(component);
        if(!bullet->targetsPlayer){
            hp -= bullet->GetDamage();
        }
    }
}