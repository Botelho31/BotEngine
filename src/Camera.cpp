#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/Player.h"

GameObject* Camera::focus;
Vec2 Camera::pos;
Vec2 Camera::speed;
Vec2 Camera::limit;
Vec2 Camera::window;

void Camera::Follow(GameObject* newFocus){
    focus = newFocus;
}

void Camera::UnFollow(){
    focus = nullptr;
}

void Camera::Update(float dt){
    InputManager *input = &(InputManager::GetInstance());
    if(focus){
        if( ((limit.x - (focus->box.x + focus->box.w)) >= (window.x/2 - focus->box.w/2)) &&
            (focus->box.x >= (window.x/2 - focus->box.w/2)) ){
            pos.x  = focus->box.x - limit.x/2 + (focus->box.w/2);

        }
        if(((limit.y - (focus->box.y + focus->box.h)) >= (window.y/2 - focus->box.h/2)) &&
        (focus->box.y >= (window.y/2 - focus->box.h/2))){
            pos.y = focus->box.y - limit.y/2 + (focus->box.h/2);
        }
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