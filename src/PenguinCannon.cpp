#include "../include/PenguinCannon.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Bullet.h"
#include "../include/Sprite.h"
#include "../include/Collider.h"

PenguinCannon::PenguinCannon(GameObject& associated,GameObject* penguinBody) : 
    Component(associated),pbody(penguinBody){
        angle = 0;
        Sprite *cannon = new Sprite(associated,"assets/img/cubngun.png");
        Collider *collider = new Collider(associated);
        associated.AddComponent(collider);
        associated.AddComponent(cannon);
}

void PenguinCannon::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    if(pbody == nullptr){
        associated.RequestDelete();
    }
    if(input->MousePress(SDL_BUTTON_LEFT) == true){
        std::cout << "shoot";
        Shoot();
    }
    // associated.box.x = pbody.lock().get()->box.x + pbody.lock().get()->box.w - associated.box.w;
    // associated.box.y = pbody.lock().get()->box.y + pbody.lock().get()->box.h - associated.box.h;
    associated.box.x = pbody->box.x + pbody->box.w/2 - associated.box.w/2;
    associated.box.y = pbody->box.y + pbody->box.h/2 - associated.box.h/2;

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
        pbody->NotifyCollision(other);
    }
}

void PenguinCannon::Shoot(){
    GameObject *bulletObj = new GameObject(&associated.GetState());
    Bullet *bullet = new Bullet(*bulletObj,-(associated.angleDeg * PI)/180,100,100,1000,"assets/img/minionbullet2.png",3,0.5,false);
    Vec2 vec = Vec2(70,0);
    vec = vec.GetRotated((associated.angleDeg * PI)/180);
    bulletObj->box.Transform(vec.x + associated.box.x + (associated.box.w/2) - 15,vec.y + associated.box.y + associated.box.h/2 - 5);
    bulletObj->AddComponent(bullet);
    associated.GetState().AddObject(bulletObj);
}