#include "../include/Collider.h"
#include "../include/Camera.h"

#define DEBUG

Collider::Collider(GameObject& associated,Vec2 scale,Vec2 offset) : 
    Component(associated),scale(scale),offset(offset){
}

void Collider::Update(float dt){
    box = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    box.w *= scale.x;
    box.h *= scale.y;
    box.x = associated.box.x + associated.box.w/2 - box.w/2 + offset.x;
    box.y = associated.box.y + associated.box.h/2 - box.h/2 + offset.y;
    // Vec2 origin = Vec2(-box.w/2,-box.h/2).GetRotated(associated.angleDeg * 180 / PI); 
    // box.Transform(origin.x + box.x,origin.y + box.y);
}


void Collider::Render() {
#ifdef DEBUG
	Vec2 center( box.GetCenter() );
	SDL_Point points[5];

	Vec2 point = (Vec2(box.x, box.y) - center).GetRotated( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y) - center).GetRotated( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[1] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[2] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x, box.y + box.h) - center).GetRotated( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Collider::Is(std::string type){
    if(type == "Collider"){
        return true;
    }else{
        return false;
    }
}

void Collider::SetScale(Vec2 scale){
    this->scale.Transform(scale);
}

void Collider::SetOffSet(Vec2 offset){
    this->offset.Transform(offset);
}