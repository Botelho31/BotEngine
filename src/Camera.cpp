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
    if(focus){
        speed.x = 100;
        speed.y = 100;
        // pos.Follow(focus->box.x - 512 + (focus->box.w/2),focus->box.y - 300 + (focus->box.h/2),speed.x,speed.y,dt);
        pos.x  = focus->box.x - 512 + (focus->box.w/2);
        pos.y = focus->box.y - 300 + (focus->box.h/2);
    }else{
        speed.x = 100;
        speed.y = 100;
        if(input->IsKeyDown(UP_ARROW_KEY)){
            pos.y -= dt * speed.y;
        }
        if(input->IsKeyDown(RIGHT_ARROW_KEY)){
            pos.x += dt * speed.x;
        }
        if(input->IsKeyDown(LEFT_ARROW_KEY)){
            pos.x -= dt * speed.x;
        }
        if(input->IsKeyDown(DOWN_ARROW_KEY)){
            pos.y += dt * speed.y;
        }
    }

}