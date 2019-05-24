#include "../include/Eye.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"
#include "../include/Player.h"
#include "../include/Camera.h"

Eye::Eye(GameObject& associated,Circle bounds,int pupilradius,bool keepPupilIn) : 
    Component(associated){
    pupil.radius = pupilradius;
    this->bounds = bounds;
    this->keepPupilIn = keepPupilIn;
    associated.box.Transform(bounds.x,bounds.y);
    this->pupil.Transform(associated.box.GetOrigin());
    originalorigin = associated.box.GetOrigin();
}

Eye::~Eye(){
}

void Eye::Update(float dt){
    Vec2 offset = originalorigin.Added(-Camera::pos.x * parallaxvalue,-Camera::pos.y * parallaxvalue);
    associated.box.Transform(offset.x,offset.y);


    Vec2 boundsbefore  = bounds.GetCenter();
    bounds.Transform(associated.box.GetOrigin());
    Vec2 boundsafter = bounds.GetCenter();
    pupil.x += boundsafter.x - boundsbefore.x;
    pupil.y += boundsafter.y - boundsbefore.y;

    PupilFollow(Player::player->GetPosition().Added(-Camera::pos.x,-Camera::pos.y),200,dt);

    float adjustdist = bounds.radius - pupil.GetDistanceFromCenter(bounds.x,bounds.y);
    float angle = bounds.GetAngleFromCenter(pupil.x,pupil.y);
    if(keepPupilIn){
        adjustdist = fabs(bounds.radius - pupil.radius) - pupil.GetDistanceFromCenter(bounds.x,bounds.y);
    }
    if((!bounds.IsInside(pupil) && keepPupilIn) || (!bounds.IsInside(pupil.GetCenter()) && !keepPupilIn)){
        if(pupil.x < bounds.x){
            pupil.x += std::fabs(adjustdist * cos(angle));
        }else{
            pupil.x -= std::fabs(adjustdist * cos(angle));
        }
        if(pupil.y < bounds.y){
            pupil.y += std::fabs(adjustdist * sin(angle));
        }else{
            pupil.y -= std::fabs(adjustdist * sin(angle));
        }
    }
}

void Eye::SetParallax(float value){
    this->parallaxvalue = value;
}

Vec2 Eye::PupilFollow(Vec2 dest,float constspeed,float dt){
    Vec2 delta;
    float angle = pupil.GetAngleFromCenter(dest.x,dest.y);
    speed.x = abs(constspeed * cos(angle));
    speed.y = abs(constspeed * sin(angle));
    if(pupil.x == dest.x){
        pupil.x = dest.x;
        delta.x = 0;
    }
    else if(pupil.x < dest.x){
        pupil.x += speed.x * dt;
        delta.x = speed.x * dt;
        if(pupil.x > dest.x){
            pupil.x = dest.x;
            delta.x = 0;
        }
    }else{
        pupil.x -= speed.x * dt;
        delta.x = -(speed.x * dt);
        if(pupil.x < dest.x){
            pupil.x = dest.x;
            delta.x = 0;
        }
    }


    if(pupil.y == dest.y){
        pupil.y = dest.y;
        delta.y = 0;
    }
    else if(pupil.y < dest.y){
        pupil.y += speed.y * dt;
        delta.y = speed.y * dt;
        if(pupil.y > dest.y){
            pupil.y = dest.y;
            delta.y = 0;
        }
    }else{
        pupil.y -= speed.y * dt;
        delta.y = -(speed.y * dt);
        if(pupil.y < dest.y){
            pupil.y = dest.y;
            delta.y = 0;
        }
    }
}

void Eye::Render() { // .Added(-Camera::pos.x,- Camera::pos.y)
    if(!keepPupilIn){
         WindowEffects::FillCircleIfInside(pupil,bounds);
    }else{
        WindowEffects::FillCircle(pupil,0,0,0,255);
    }
#ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS)){
        WindowEffects::DrawCircle(bounds,0,0,0,255);
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