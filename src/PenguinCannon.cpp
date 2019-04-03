#include "../include/PenguinCannon.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Bullet.h"
#include "../include/Sprite.h"
#include "../include/Collider.h"

PenguinCannon::PenguinCannon(GameObject& associated,std::weak_ptr<GameObject> penguinBody) : 
    Component(associated),pbody(penguinBody){
        angle = 0;
        timer = new Timer();
        Sprite *cannon = new Sprite(associated,"assets/img/cubngun.png");
        Collider *collider = new Collider(associated);
        associated.AddComponent(collider);
        associated.AddComponent(cannon);
}

PenguinCannon::~PenguinCannon(){
    delete timer;
}

void PenguinCannon::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    timer->Update(dt);
    if(pbody.lock().get() == nullptr){
        associated.RequestDelete();
    }
    if((input->MousePress(SDL_BUTTON_LEFT) == true) && (timer->Get() >= 0.4)){
        timer->Restart();
        Shoot();
    }
    associated.box.x = pbody.lock()->box.x + pbody.lock()->box.w/2 - associated.box.w/2;
    associated.box.y = pbody.lock()->box.y + pbody.lock()->box.h/2 - associated.box.h/2;

    float angle = associated.box.GetAngle(input->GetMouseX() + Camera::pos.x,input->GetMouseY() + Camera::pos.y,associated.box.w/2,associated.box.h/2);
    associated.angleDeg = -(angle * 180)/PI;
}

void PenguinCannon::Render(){

}

bool PenguinCannon::Is(std::string type){
    if(type == "PenguinCannon"){
        return true;
    }else{
        return false;
    }
}

void PenguinCannon::NotifyCollision(GameObject& other){
    Component *component = other.GetComponent("Bullet");
    if(component){
        pbody.lock()->NotifyCollision(other);
    }
}

void PenguinCannon::Shoot(){
    GameObject *bulletObj = new GameObject();
    Bullet *bullet = new Bullet(*bulletObj,-(associated.angleDeg * PI)/180,100,100,1000,"assets/img/minionbullet2.png",3,0.5,false);
    Vec2 vec = Vec2(70,0);
    vec = vec.GetRotated((associated.angleDeg * PI)/180);
    bulletObj->box.Transform(vec.x + associated.box.x + (associated.box.w/2) - 15,vec.y + associated.box.y + associated.box.h/2 - 5);
    bulletObj->AddComponent(bullet);
    Game::GetInstance().GetCurrentState().AddObject(bulletObj);
}