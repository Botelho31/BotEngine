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
    #ifdef DEBUG
        if(input->IsKeyDown(SDLK_0)){
            std::cout << "MouseX: " << input->GetMouseX() * 2  + Camera::pos.x<< std::endl;
            std::cout << "MouseY: " << input->GetMouseY() * 2  + Camera::pos.y << std::endl;
        }
    #endif
    if(focus){
        if( ((limit.x - (focus->box.x + focus->box.w/2)) >= window.x/2) &&
            ((focus->box.x + focus->box.w/2) >= (window.x/2)) ){
            pos.x  = focus->box.x - window.x/2 + (focus->box.w/2);

        }else if((focus->box.x + focus->box.w/2) < (window.x/2)){
            pos.x = 0;
        }else if((limit.x - (focus->box.x + focus->box.w/2)) < window.x/2){
            pos.x = limit.x - window.x;
        }
        if(((limit.y - (focus->box.y + focus->box.h/2)) >= (window.y/2)) &&
        ((focus->box.y + focus->box.h/2) >= (window.y/2))){
            pos.y = focus->box.y - window.y/2 + (focus->box.h/2);
        }else if((focus->box.y + focus->box.h/2) < (window.y/2)){
            pos.y = 0;
        }else if((limit.y - (focus->box.y + focus->box.h)) < (window.y/2 - focus->box.h/2)){
            pos.y = limit.y - window.y;
        }
    }else{
        speed.x = 300;
        speed.y = 300;
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

void Camera::Correct(){
    if((pos.x + window.x) > limit.x){
        pos.x = limit.x - window.x;
    }
    if(pos.x < 0){
        pos.x = 0;
    }
    if((pos.y + window.y) > limit.y){
        pos.y = limit.y - window.y;
    }
    if(pos.y < 0){
        pos.y = 0;
    }
}