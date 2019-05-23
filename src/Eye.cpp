#include "../include/Eye.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"
#include "../include/Player.h"
#include "../include/Camera.h"

Eye::Eye(GameObject& associated,int radius) : 
    Component(associated){
    this->physics = new Physics(associated,&speed,true,true);
    associated.AddComponent(physics);
    this->radius = radius;
    originalpos = associated.box.GetCenter();


}

Eye::~Eye(){
}

void Eye::Update(float dt){
    physics->Follow(Player::player->GetPosition(),100,dt);
    float angle = associated.box.GetAngle(originalpos.x,originalpos.y);
    if(associated.box.GetCenter().GetDistance(originalpos.x,originalpos.y) > radius){
        if(associated.box.x < originalpos.x){
            associated.box.x += std::fabs((radius - associated.box.GetCenter().GetDistance(originalpos.x,originalpos.y)) * cos(angle));
        }else{
            associated.box.x -= std::fabs((radius - associated.box.GetCenter().GetDistance(originalpos.x,originalpos.y)) * cos(angle));
        }
        if(associated.box.GetCenter().y < originalpos.y){
            associated.box.y += std::fabs((radius - associated.box.GetCenter().GetDistance(originalpos.x,originalpos.y)) * sin (angle));
        }else{
            associated.box.y -= std::fabs((radius - associated.box.GetCenter().GetDistance(originalpos.x,originalpos.y)) * sin (angle));
        }
        
    }
}

void Eye::Render() {
    WindowEffects::FillCircleIfInside(associated.box.x - Camera::pos.x,associated.box.y - Camera::pos.y,10,originalpos.x - Camera::pos.x,originalpos.y - Camera::pos.y,radius);
#ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS)){
        WindowEffects::DrawCircle(originalpos.x - Camera::pos.x,originalpos.y - Camera::pos.y,radius);
	}
#endif // DEBUG
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