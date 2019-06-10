#include "../include/InputManager.h"
#include "../include/Camera.h"

InputManager::InputManager(){
    quitRequested = false;
    mouseX = 0;
    mouseY = 0;
    updateCouter = 0;

    //GAME CONTROLLER INITIATING
    // std::cout << "Controller test" << std::endl;
    // for(int i = 0;i < SDL_NumJoysticks();i++){
    //     if(SDL_IsGameController(i)){
    //         controller = SDL_GameControllerOpen(i);
    //         std::cout << "Controller Detected" << std::endl;
    //         break;
    //     }
    //     std::cout << SDL_GetError() << std::endl;
    // }
    
    for(int i = 0; i < 6; i++){
        mouseState[i] = false;
        mouseUpdate[i] = 0;
    }
    for(int i = 0;i < 416; i++){
        keyState[i] = false;
        keyUpdate[i] = 0;
    }
}

InputManager& InputManager::GetInstance(){
    static InputManager *instance = new InputManager();
    return *instance;
}

InputManager::~InputManager(){

}

void InputManager::Update(){
    SDL_Event event;
	SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    updateCouter ++;

	while (SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			quitRequested = true;
		}
        if(!(event.key.repeat)){
            //GAME CONTROLLER TESTING
            // if(event.type == SDL_CONTROLLERBUTTONDOWN){
            //     if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
            //         std::cout << "A" << std::endl;
            //     }
            // }
            //END CONTROLLER TESTING
            if(event.type == SDL_MOUSEBUTTONUP) {
                mouseState[event.button.button] = false;
                mouseUpdate[event.button.button] = updateCouter;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                mouseState[event.button.button] = true;
                mouseUpdate[event.button.button] = updateCouter;
            }
            if( event.type == SDL_KEYUP ) {
                int key = event.key.keysym.sym;
                if(key > 127){
                    key -= 1073741697;
                }
                keyState[key] = false;
                keyUpdate[key] = updateCouter;
            }
            if( event.type == SDL_KEYDOWN ) {
                int key = event.key.keysym.sym;
                if(key > 127){
                    key -= 1073741697;
                }
                keyState[key] = true;
                keyUpdate[key] = updateCouter;
            }
        }
	}
}


bool InputManager::KeyPress(int key){
    if(key > 416){
        key -= 1073741697;
    }
    if((keyUpdate[key] == updateCouter) && (keyState[key] == true)){
        return true;
    }else{
        return false;
    }
}

bool InputManager::KeyRelease(int key){
    if(key > 416){
        key -= 1073741697;
    }
    if((keyUpdate[key] == updateCouter) && (keyState[key] == false)){
        return true;
    }else{
        return false;
    }
}

bool InputManager::IsKeyDown(int key){
    if(key > 416){
        key -= 1073741697;
    }
    return keyState[key];
}

bool InputManager::MousePress(int button){
    if((mouseUpdate[button] == updateCouter) && (mouseState[button] == true)){
        return true;
    }else{
        return false;
    }
}

bool InputManager::MouseRelease(int button){
    if((mouseUpdate[button] == updateCouter) && (mouseState[button] == false)){
        return true;
    }else{
        return false;
    }
}

bool InputManager::IsMouseDown(int button){
    return mouseState[button];
}

int InputManager::GetMouseX(){
    return mouseX;
}

int InputManager::GetMouseY(){
    return mouseY;
}

bool InputManager::QuitRequested(){
    return quitRequested;
}
