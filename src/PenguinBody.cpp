#include "../include/PenguinBody.h"
#include "../include/PenguinCannon.h"
#include "../include/Collider.h"
#include "../include/Bullet.h"
#include "../include/Camera.h"

PenguinBody *PenguinBody::player;

PenguinBody::PenguinBody(GameObject& associated) : Component(associated){
    speed.x = 0;
    speed.y = 0;
    linearSpeed = 0;
    angle = 0;
    hp = 150;
    player = this;
    Sprite *penguin = new Sprite(associated,"assets/img/penguin.png");
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(penguin);
}

PenguinBody::~PenguinBody(){
    player = nullptr;

}

void PenguinBody::Start(){
    // std::shared_ptr<GameObject> sharedptr(&associated);      //TEMPORARY AS POINTER
    // std::weak_ptr<GameObject> weakptr = sharedptr;
    GameObject *cannonObj = new GameObject(&(associated.GetState()));
    PenguinCannon *cannon = new PenguinCannon(*cannonObj,&associated);
    // std::shared_ptr<GameObject> sharedcannon(cannonObj);
    // std::weak_ptr<GameObject> weakcannon = sharedcannon;
    pcannon = cannonObj;
    cannonObj->AddComponent(cannon);
    associated.GetState().AddObject(cannonObj);
}

void PenguinBody::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    if(input->IsKeyDown(UP_ARROW_KEY) == true){
        speed.x += 50 * dt;
        speed.y += 50 * dt;
        linearSpeed += 50 * dt;
        if(speed.x >= 300){
            speed.x = 300;
        }
        if(speed.y >= 300){
            speed.y = 300;
        }
        if(linearSpeed >= 300){
            linearSpeed = 300;
        }
    }
    if(input->IsKeyDown(DOWN_ARROW_KEY) == true){
        speed.x -= 50 * dt;
        speed.y -= 50 * dt;
        linearSpeed -= 50 * dt;
        if(speed.x <= -300){
            speed.x = -300;
        }
        if(speed.y <= -300){
            speed.y = -300;
        }
        if(linearSpeed <= -300){
            linearSpeed = -300;
        }
    }
    if(input->IsKeyDown(RIGHT_ARROW_KEY) == true){
        angle += PI/3 * dt;
        associated.angleDeg = ( angle * 180 )/ PI;
    }
    if(input->IsKeyDown(LEFT_ARROW_KEY) == true){
        angle -= PI/3 * dt;
        associated.angleDeg = ( angle * 180 )/ PI;
    }
    associated.box.x += (cos(angle) * speed.x) * dt;
    associated.box.y += -((-sin(angle)) * speed.y) * dt;

    if(hp <= 0){
        Camera::UnFollow();
        pcannon->RequestDelete();
        associated.RequestDelete();
    }
}

void PenguinBody::Render(){

}

bool PenguinBody::Is(std::string type){
    if(type == "PenguinBody"){
        return true;
    }else{
        return false;
    }
}

void PenguinBody::NotifyCollision(GameObject& other){
    Component *component = other.GetComponent("Bullet");
    if(component){
        Bullet *bullet = dynamic_cast<Bullet*>(component);
        if(bullet->targetsPlayer){
            hp -= bullet->GetDamage();
        }
    }
}