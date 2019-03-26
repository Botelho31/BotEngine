#include "../include/Camera.h"
#include "../include/InputManager.h"

GameObject* Camera::focus;
Vec2 Camera::pos;
Vec2 Camera::speed;

void Camera::Follow(GameObject* newFocus){
    focus = newFocus;
}

void Camera::UnFollow(){
    focus = nullptr;
}

void Camera::Update(float dt){
    InputManager *input = &(InputManager::GetInstance());
    if(input->IsKeyDown(UP_ARROW_KEY)){
        pos.y += 5;
    }
    if(input->IsKeyDown(RIGHT_ARROW_KEY)){
        pos.x -= 5;
    }
    if(input->IsKeyDown(LEFT_ARROW_KEY)){
        pos.x += 5;
    }
    if(input->IsKeyDown(DOWN_ARROW_KEY)){
        pos.y -= 5;
    }

}