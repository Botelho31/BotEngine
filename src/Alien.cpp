#include "../include/Alien.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Resources.h"
#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Bullet.h"
#include "../include/Sound.h"

Alien::Alien(GameObject& associated,int nMinions) : Component(associated){
    speed.x = 150;
    speed.y = 150;
    hp = 30;
    this->nMinions = nMinions;
    Sprite *alien = new Sprite(associated,"assets/img/alien.png");
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(alien);
}

Alien::Action::Action(Action::ActionType type,float x,float y) : type(type),pos(x,y){
}

Alien::~Alien(){
    // std::queue<Action> clear;
    // taskQueue.swap(clear);
}

void Alien::Start(){
    for(int i = 0; i < nMinions;i ++){
        GameObject *minionObj = new GameObject(&associated.GetState());
        std::weak_ptr<GameObject> alienCenter = associated.GetState().GetObjectPtr(&associated);
        Minion *minion = new Minion(*minionObj,alienCenter,i*(PI*2/(float)nMinions));
        Component *component = minionObj->GetComponent("Sprite");
        if(component != nullptr){
            Sprite *sprite = dynamic_cast<Sprite*>(component);
            float scale = (rand() % 5) + 10;
            scale /= 10;
            sprite->SetScaleX(scale,scale);
        }
        minionObj->AddComponent(minion);
        std::weak_ptr<GameObject> minionWeakPtr = associated.GetState().AddObject(minionObj);
        minionArray.emplace_back(minionWeakPtr);

    }
}

void Alien::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    associated.angleDeg -= (180/10) * dt;
    if(input->MousePress(SDL_BUTTON_LEFT) == true){
        // Action::ActionType actiontype = Action::MOVE;
        // Action *action = new Action(actiontype,(input->GetMouseX() + Camera::pos.x) - associated.box.w/2,(input->GetMouseY() + Camera::pos.y) - associated.box.h/2);
        // taskQueue.emplace(*action);
    }
    else if(input->MousePress(SDL_BUTTON_RIGHT) == true){
        Action::ActionType actiontype = Action::SHOOT;
        Action *action = new Action(actiontype,input->GetMouseX() + Camera::pos.x,input->GetMouseY() + Camera::pos.y);
        taskQueue.emplace(*action);
    }
    if(!(taskQueue.empty())){
        if(taskQueue.front().type == Action::MOVE){
            float x = taskQueue.front().pos.x;
            float y = taskQueue.front().pos.y;
            if(associated.box.Follow(x,y,speed.x,speed.y,dt)){
                taskQueue.pop();
            }
        }   
        if(taskQueue.front().type == Action::SHOOT){
            float lastDistance = minionArray[0].lock().get()->box.GetDistance(taskQueue.front().pos.x,taskQueue.front().pos.y);
            int chosen = 0;
            for(int i = 0; i < minionArray.size();i++){
                float distance = minionArray[i].lock().get()->box.GetDistance(taskQueue.front().pos.x,taskQueue.front().pos.y);
                if(distance < lastDistance){
                    lastDistance = distance;
                    chosen = i;
                }
            }
            Component* component = minionArray[chosen].lock().get()->GetComponent("Minion");
            if(component){
                Minion *minion = dynamic_cast<Minion*>(component);
                minion->Shoot(taskQueue.front().pos);
            }
            taskQueue.pop();
        }
    }
    if(hp <= 0){
        GameObject *explosionObj = new GameObject(&associated.GetState());
        Sprite *explosion = new Sprite(*explosionObj,"assets/img/aliendeath.png",4,0.2,0.8);
        explosionObj->box.Transform(associated.box.x + associated.box.w/2 - explosionObj->box.w/2,associated.box.y + associated.box.h/2 - explosionObj->box.h/2);
        explosionObj->angleDeg = associated.angleDeg;
        Sound *sound =  new Sound(*explosionObj,"assets/audio/boom.wav");
        sound->Play(1);
        explosionObj->AddComponent(explosion);
        explosionObj->AddComponent(sound);
        associated.GetState().AddObject(explosionObj);
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