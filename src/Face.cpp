#include "../include/Face.h"
#include "../include/Sound.h"

    Face::Face(GameObject& associated) : Component(associated){
        hitpoints = 30;
    }

    Face::~Face(){
        
    }

    void Face::Damage(int damage){
        hitpoints -= damage;
        if(hitpoints <= 0){
            Component *component = associated.GetComponent("Sound");
            if(component != nullptr){
                Sound *sound = dynamic_cast<Sound*>(component);
                sound->Play();
            }
            associated.RequestDelete();
        }
    }

    void Face::Update(float dt){

    }

    void Face::Render(){

    }

    bool Face::Is(std::string type){
        if(type == "Face"){
            return true;
        }else{
            return false;
        }
    }