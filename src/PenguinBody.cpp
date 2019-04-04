#include "../include/PenguinBody.h"
#include "../include/PenguinCannon.h"
#include "../include/Collider.h"
#include "../include/Bullet.h"
#include "../include/Camera.h"
#include "../include/Sound.h"
#include "../include/Sprite.h"

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
    std::weak_ptr<GameObject> weakbody = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    GameObject *cannonObj = new GameObject();
    PenguinCannon *cannon = new PenguinCannon(*cannonObj,weakbody);
    cannonObj->AddComponent(cannon);
    pcannon = Game::GetInstance().GetCurrentState().AddObject(cannonObj);
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
    if (((associated.box.x >= 0) && (associated.box.x <= 1408)) && ((associated.box.y >= 0) && (associated.box.y <= 1280))){
        associated.box.x += (cos(angle) * speed.x) * dt;
        associated.box.y += -((-sin(angle)) * speed.y) * dt;
    }

    if(hp <= 0){
        GameObject *explosionObj = new GameObject();
        Sprite *explosion = new Sprite(*explosionObj,"assets/img/penguindeath.png",5,0.2,1.0);
        explosionObj->box.Transform(associated.box.x + associated.box.w/2 - explosionObj->box.w/2,associated.box.y + associated.box.h/2 - explosionObj->box.h/2);
        Sound *sound =  new Sound(*explosionObj,"assets/audio/boom.wav");
        sound->Play(1);
        explosionObj->AddComponent(explosion);
        explosionObj->AddComponent(sound);
        Game::GetInstance().GetCurrentState().AddObject(explosionObj);
        Camera::UnFollow();
        pcannon.lock()->RequestDelete();
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

Vec2 PenguinBody::GetPosition(){
    return Vec2(associated.box.x + associated.box.w/2,associated.box.y + associated.box.h/2);
}