#include "../include/GameObject.h"
#include "../include/Component.h"


GameObject::GameObject(){
    this->isDead = false;
}

GameObject::~GameObject(){
    for(unsigned int i = components.size(); i > 0; i--){
        delete components.at(i);
    }
    components.clear();
}

void GameObject::Update(float dt){
    for(unsigned int i = 0; i <= components.size();i++){
        components[i]->Update(dt);
    }
}

void GameObject::Render(){
    for(unsigned int i = 0; i <= components.size();i++){
        components[i]->Render();
    }
}

bool GameObject::IsDead(){
    return isDead;
}

void GameObject::RequestDelete(){
    isDead = true;
}

void GameObject::AddComponent(Component* cpt){
    components.push_back(cpt);
}

void GameObject::RemoveComponent(Component* cpt){
    for(unsigned int i = 0; i <= components.size();i++){
        if(components.at(i) == cpt){
            delete components.at(i);
            components.erase(components.begin() + i);
        }
    }
}

Component* GameObject::GetComponent(std::string type){
    for(unsigned int i = 0; i <= components.size();i++){
        if(components[i]->Is(type)){
            return components.at(i);
        }
    }
    return nullptr;
}

