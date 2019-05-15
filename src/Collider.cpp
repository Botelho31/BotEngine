#include "../include/Collider.h"
#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/WindowEffects.h"

Collider::Collider(GameObject& associated,Vec2 scale,Vec2 offset) : 
    Component(associated),scale(scale),offset(offset){
}

void Collider::Update(float dt){
    box = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    box.w *= scale.x;
    box.h *= scale.y;
    box.x = associated.box.x + associated.box.w/2 - box.w/2 + offset.x;
    box.y = associated.box.y + associated.box.h/2 - box.h/2 + offset.y;
}


void Collider::Render() {
#ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS)){
		WindowEffects::DrawBox(box,ToPI(associated.angleDeg),255,0,0);
	}
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

void Collider::UpdateAssociated(){
    associated.box.x = box.x - associated.box.w/2 + box.w/2 - offset.x;
    associated.box.y = box.y - associated.box.h/2 + box.h/2 - offset.y;
}