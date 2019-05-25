#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/Player.h"

GameObject* Camera::focus;
Vec2 Camera::pos;
Vec2 Camera::speed;
Vec2 Camera::limit;
Vec2 Camera::window;

Timer* Camera::shakescreen = new Timer();
float Camera::shaketime = 0;
int Camera::shakeintensity = 10;

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
        int intensity = shakeintensity * ((shaketime - shakescreen->Get())/shaketime);
        if(intensity <= 0){
            intensity = 1;
        }
        if( ((limit.x - (focus->box.x + focus->box.w/2)) >= window.x/2) &&
            ((focus->box.x + focus->box.w/2) >= (window.x/2)) ){
            pos.x  = focus->box.x - window.x/2 + (focus->box.w/2);

            if(shakescreen->Started()){
                int randX = (rand() % (intensity * 2)) - intensity + 1;
                pos.x += randX;
            }

        }else if((focus->box.x + focus->box.w/2) < (window.x/2)){
            pos.x = 0;

            if(shakescreen->Started()){
                int randX = (rand() % intensity) + 1;
                pos.x += randX;
            }
        }else if((limit.x - (focus->box.x + focus->box.w/2)) < window.x/2){
            pos.x = limit.x - window.x;

            if(shakescreen->Started()){
                int randX = (rand() % intensity) + 1;
                pos.x -= randX;
            }
        }
        if(((limit.y - (focus->box.y + focus->box.h/2)) >= (window.y/2)) &&
        ((focus->box.y + focus->box.h/2) >= (window.y/2))){
            pos.y = focus->box.y - window.y/2 + (focus->box.h/2);

            if(shakescreen->Started()){
                int randY = (rand() % (intensity*2)) - intensity + 1;
                pos.y += randY;
            }
        }else if((focus->box.y + focus->box.h/2) < (window.y/2)){
            pos.y = 0;

            if(shakescreen->Started()){
                int randY = (rand() % intensity) + 1;
                pos.y += randY;
            }
        }else if((limit.y - (focus->box.y + focus->box.h)) < (window.y/2 - focus->box.h/2)){
            pos.y = limit.y - window.y;

            if(shakescreen->Started()){
                int randY = (rand() % intensity) + 1;
                pos.y -= randY;
            }
        }

        if(shakescreen->Started()){
            shakescreen->Update(dt);
            if(shakescreen->Get() >= shaketime){
                shakescreen->Restart();
            }
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

void Camera::ShakeScreen(float time,int intensity){
    shaketime = time;
    shakeintensity = intensity;
    shakescreen->Delay(0);
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