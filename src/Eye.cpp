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
    associated.box.w = 5;
    associated.box.h = 5;
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
    WindowEffects::FillRect(associated.box.Added(-Camera::pos.x,-Camera::pos.y,0,0),0,0,0,255);
    WindowEffects::DrawCircle(originalpos.x - Camera::pos.x,originalpos.y - Camera::pos.y,radius);
    // WindowEffects::FillRect(associated.box,0,0,0,255);
#ifdef DEBUG
	// InputManager *input = &(InputManager::GetInstance());
	// if(input->IsKeyDown(SDLK_EQUALS)){
    //     Rect eyelimit = Rect(originalpos.Added(-radius,-radius).x,originalpos.Added(-radius,-radius).y,2*radius,2*radius);
        // WindowEffects::DrawBox(eyelimit,ToPI(associated.angleDeg),0,0,0);
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