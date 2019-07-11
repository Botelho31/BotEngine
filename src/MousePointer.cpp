#include "../include/MousePointer.h"
#include "../include/Collider.h"
#include "../include/WindowEffects.h"
#include "../include/Camera.h"

MousePointer::MousePointer(GameObject& associated) : Component(associated){
    associated.box.w = 5;
    associated.box.h = 5;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

MousePointer::~MousePointer(){

}

void MousePointer::Update(float dt){
    InputManager *input = &(InputManager::GetInstance());
    Vec2 mouse = Vec2(input->GetMouseX() * 2 + Camera::pos.x,input->GetMouseY() * 2 + Camera::pos.y);
    associated.box.SetCenter(mouse);
    if(input->IsKeyDown(SDLK_EQUALS)){
        std::cout << mouse.x << " " << mouse.y << std::endl;
    }
}

void MousePointer::Render(){

}

bool MousePointer::Is(std::string type){
    if(type == "MousePointer"){
        return true;
    }else{
        return false;
    }
}

void MousePointer::NotifyCollision(GameObject& other){

}

bool MousePointer::IsPressed(){
    InputManager *input = &(InputManager::GetInstance());
    return input->IsMouseDown(LEFT_MOUSE_BUTTON);
}