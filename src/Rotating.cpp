#include "../include/Rotating.h"


Rotating::Rotating(GameObject& associated,int rotatingSum) : Component(associated){
    this->rotatingSum = rotatingSum;
}

Rotating::~Rotating(){

}

void Rotating::Update(float dt){
    associated.angleDeg += rotatingSum;
    if(associated.angleDeg >= 360){
        associated.angleDeg = 0;
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