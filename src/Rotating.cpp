#include "../include/Rotating.h"


Rotating::Rotating(GameObject& associated,int rotatingSum) : Component(associated){
    this->rotatingSum = rotatingSum;
    this->timerrot = new Timer();
}

Rotating::~Rotating(){
    delete this->timerrot;
}

void Rotating::Update(float dt){
    timerrot->Update(dt);
    if(timerrot->Get() >= 0.04){
        associated.angleDeg += rotatingSum;
        if(associated.angleDeg >= 360){
            associated.angleDeg = 0;
        }
        timerrot->Restart();
    }
}

void Rotating::Render(){

}

bool Rotating::Is(std::string type){
    if(type == "Rotating"){
        return true;
    }else{
        return false;
    }
}