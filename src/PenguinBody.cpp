#include "../include/PenguinBody.h"
#include "../include/PenguinCannon.h"

PenguinBody *PenguinBody::player;
PenguinBody::PenguinBody(GameObject& associated) : Component(associated){
    speed.x = 150;
    speed.y = 150;
    linearSpeed = 150;
    angle = 0;
    hp = 150;
    player = this;
    Sprite *penguin = new Sprite(associated,"assets/img/penguin.png");
    associated.AddComponent(penguin);
}

PenguinBody::~PenguinBody(){
    player = nullptr;

}

void PenguinBody::Start(){
    std::shared_ptr<GameObject> sharedptr(&associated);
    std::weak_ptr<GameObject> weakptr = sharedptr;
    GameObject *cannonObj = new GameObject(&associated.GetState());
    PenguinCannon *cannon = new PenguinCannon(associated,weakptr);
    std::shared_ptr<GameObject> sharedcannon(cannonObj);
    std::weak_ptr<GameObject> weakcannon = sharedcannon;
    pcannon = weakcannon;
    cannonObj->AddComponent(cannon);
    associated.GetState().AddObject(cannonObj);

}

void PenguinBody::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    if(input->MousePress(UP_ARROW_KEY) == true){
        speed.x += 10 * dt;
        speed.y += 10 * dt;
        linearSpeed += 10 * dt;
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
    else if(input->MousePress(DOWN_ARROW_KEY) == true){
        speed.x -= 10 * dt;
        speed.y -= 10 * dt;
        linearSpeed -= 10 * dt;
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
    else if(input->MousePress(RIGHT_ARROW_KEY) == true){
        angle += PI/10 * dt;
        associated.angleDeg = angle;
    }
    else if(input->MousePress(LEFT_ARROW_KEY) == true){
        angle -= PI/10 * dt;
        associated.angleDeg = ( angle * 180 )/ PI;
    }
    associated.box.x += (cos(angle) * speed.x) * dt;
    associated.box.y += ((-sin(angle)) * speed.y) * dt;

    if(hp <= 0){
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