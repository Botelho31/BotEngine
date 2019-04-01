#include "../include/PenguinCannon.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Bullet.h"
#include "../include/Sprite.h"

PenguinCannon::PenguinCannon(GameObject& associated,std::weak_ptr<GameObject> penguinBody) : 
    Component(associated),pbody(penguinBody){
        angle = 0;
        Sprite *cannon = new Sprite(associated,"assets/img/cubngun.png");


}

void PenguinCannon::Update(float dt){
    if(pbody.lock().get() == nullptr){
        associated.RequestDelete();
    }
    associated.box.x = pbody.lock().get()->box.x + pbody.lock().get()->box.w - associated.box.w;
    associated.box.y = pbody.lock().get()->box.y + pbody.lock().get()->box.h - associated.box.h;

    InputManager *input =  &(InputManager::GetInstance());

    float angle = associated.box.GetAngle(input->GetMouseX() + Camera::pos.x,input->GetMouseY() + Camera::pos.y,associated.box.w/2,associated.box.h/2);

    associated.angleDeg = (angle * 180)/PI;
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

void PenguinCannon::Shoot(){
    GameObject *bulletObj = new GameObject(&associated.GetState());
    Bullet *bullet = new Bullet(*bulletObj,(associated.angleDeg * PI)/180,100,100,1000,"assets/img/minionbullet2.png",3,0.5);
    bulletObj->box.Transform(associated.box.x + associated.box.w,associated.box.y + associated.box.h/2 - bulletObj->box.h/2); 
    bulletObj->AddComponent(bullet);
    associated.GetState().AddObject(bulletObj);
}