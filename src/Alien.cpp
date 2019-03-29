#include "../include/Alien.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Resources.h"
#include "../include/Minion.h"

Alien::Alien(GameObject& associated,int nMinions) : Component(associated){
    speed.x = 150;
    speed.y = 150;
    hp = 30;
    this->nMinions = nMinions;
    minionArray.clear();
    Sprite *alien = new Sprite(associated,"assets/img/alien.png");
    associated.AddComponent(alien);
}

Alien::Action::Action(Action::ActionType type,float x,float y) : type(type),pos(x,y){
}

Alien::~Alien(){
    minionArray.clear();
    while(!taskQueue.empty()){
        taskQueue.pop();
    }
}

void Alien::Start(){
    for(int i = 0; i < nMinions;i ++){
        // GameObject *minionObj = new GameObject(&associated.GetState());
        // std::weak_ptr<GameObject> alienCenter = associated.GetState().GetObjectPtr(&associated);
        // Minion *minion = new Minion(*minionObj,alienCenter,i*(PI*2/(float)nMinions));
        // minionObj->AddComponent(minion);
        // associated.GetState().AddObject(minionObj);
        // minionArray.emplace_back(*minionObj);
    }
}

void Alien::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    if(input->MousePress(SDL_BUTTON_LEFT) == true){
        Action::ActionType actiontype = Action::MOVE;
        Action *action = new Action(actiontype,(input->GetMouseX() + Camera::pos.x),(input->GetMouseY() + Camera::pos.y));
        taskQueue.emplace(*action);
    }
    else if(input->MousePress(SDL_BUTTON_RIGHT) == true){
        Action::ActionType actiontype = Action::SHOOT;
        Action *action = new Action(actiontype,input->GetMouseX() + Camera::pos.x,input->GetMouseY() + Camera::pos.y);
        taskQueue.emplace(*action);
    }
    if(!(taskQueue.empty())){
        if(taskQueue.front().type == Action::MOVE){
            float x = taskQueue.front().pos.x;
            float y = taskQueue.front().pos.y;
            if(associated.box.Follow(x,y,speed.x,speed.y,dt)){
                taskQueue.pop();
            }
            // std::cout << "X: " << associated.box.x << " Y: " << associated.box.y << std::endl;
            // std::cout << "DestX: " << x << " DestY: " << y << std::endl;
        }   
        if(taskQueue.front().type == Action::SHOOT){
            taskQueue.pop();
        }
    }
    if(hp <= 0){
        associated.RequestDelete();
    }

}

void Alien::Render(){

}

bool Alien::Is(std::string type){
    if(type == "Alien"){
        return true;
    }else{
        return false;
    }
}