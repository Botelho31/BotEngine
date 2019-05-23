#include "../include/Eye.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"
#include "../include/Player.h"
#include "../include/Camera.h"

Eye::Eye(GameObject& associated,Circle bounds,int pupilradius) : 
    Component(associated){
    this->physics = new Physics(associated,&speed,true,true);
    associated.AddComponent(physics);
    pupil.radius = pupilradius;
    this->bounds = bounds;
    associated.box.Transform(bounds.x,bounds.y);
    this->pupil.Transform(associated.box.GetOrigin());
}

Eye::~Eye(){
}

void Eye::Update(float dt){
    physics->Follow(Player::player->GetPosition(),100,dt);
    this->pupil.Transform(associated.box.GetOrigin());

    float angle = bounds.GetAngleFromCenter(pupil.x,pupil.y);
    if(!bounds.IsInside({pupil.x,pupil.y})){
        if(associated.box.x < bounds.x){
            associated.box.x += std::fabs((bounds.radius - associated.box.GetCenter().GetDistance(bounds.x,bounds.y)) * cos(angle));
        }else{
            associated.box.x -= std::fabs((bounds.radius - associated.box.GetCenter().GetDistance(bounds.x,bounds.y)) * cos(angle));
        }
        if(associated.box.GetCenter().y < bounds.y){
            associated.box.y += std::fabs((bounds.radius - associated.box.GetCenter().GetDistance(bounds.x,bounds.y)) * sin (angle));
        }else{
            associated.box.y -= std::fabs((bounds.radius - associated.box.GetCenter().GetDistance(bounds.x,bounds.y)) * sin (angle));
        }
        this->pupil.Transform(associated.box.GetOrigin());
    }
}

void Eye::Render() {
    WindowEffects::FillCircleIfInside(pupil.Added(-Camera::pos.x,-Camera::pos.y),bounds.Added(-Camera::pos.x,-Camera::pos.y));
#ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	// if(input->IsKeyDown(SDLK_EQUALS)){
        WindowEffects::DrawCircle(bounds.Added(-Camera::pos.x,-Camera::pos.y));
	// }
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