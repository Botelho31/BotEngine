#include "../include/GameObject.h"
#include "../include/Component.h"


GameObject::GameObject(){
    this->isDead = false;
    this->started = false;
}

GameObject::~GameObject(){
    // std::cout << "Cleared "<< components.size() << " Components" << std::endl;
    components.clear();
}

void GameObject::Start(){
    for(unsigned int i = 0; i < components.size();i++){
        components[i]->Start();
    }
    started = true;
}
void GameObject::Update(float dt){
    for(unsigned int i = 0; i < components.size();i++){
        components[i]->Update(dt);
    }
}

void GameObject::Render(){
    for(unsigned int i = 0; i < components.size();i++){
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
    components.emplace_back(cpt);
    if(started){
        cpt->Start();
    }
}

void GameObject::RemoveComponent(Component* cpt){
    for(unsigned int i = 0; i < components.size();i++){
        if(components[i].get() == cpt){
            components.erase(components.begin() + i);
        }
    }
}

Component* GameObject::GetComponent(std::string type){
    for(unsigned int i = 0; i < components.size();i++){
        if(components[i]->Is(type)){
            return components[i].get();
        }
    }
    return nullptr;
}

void GameObject::NotifyCollision(GameObject &other){
    for(unsigned int i = 0; i < components.size();i++){
        components[i]->NotifyCollision(other);
    }
}

