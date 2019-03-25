#include "../include/Face.h"
#include "../include/Sound.h"

    Face::Face(GameObject& associated) : Component(associated){
        hitpoints = 30;
        isDying = false;
    }

    Face::~Face(){
        
    }

    void Face::Damage(int damage){
        hitpoints -= damage;
        if((hitpoints <= 0) && (isDying == false)){
            Component *component = associated.GetComponent("Sound");
            if(component != nullptr){
                Sound *sound = dynamic_cast<Sound*>(component);
                sound->Play(1);
                isDying = true;
                SDL_TimerID timerid =  SDL_AddTimer(800,DelayedDeath,&associated);
            }else{
                associated.RequestDelete();
            }
        }
    }

    Uint32 Face::DelayedDeath(Uint32 interval, void *param){
        GameObject* game =  static_cast<GameObject*>(param);
        game->RequestDelete();
        return 0;
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