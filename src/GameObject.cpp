#include "../include/GameObject.h"
#include "../include/Component.h"
#include "../include/Physics.h"


GameObject::GameObject(){
    this->isDead = false;
    this->started = false;
    this->angleDeg = 0;
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
        if(components[i]->GetFreeze()){
            components[i]->UpdateFreeze(dt);
        }else{
            components[i]->Update(dt);
        }
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

std::weak_ptr<Component> GameObject::AddComponent(Component* cpt){
    std::shared_ptr<Component> component(cpt);
    components.emplace_back(component);
    if(started){
        cpt->Start();
    }
    std::weak_ptr<Component> weakptr = component;
    return weakptr;
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

Physics* GameObject::GetPhysics(){
    for(unsigned int i = 0; i < components.size();i++){
        if(components[i]->Is("Physics")){
            Physics *physics = dynamic_cast<Physics*>(components[i].get());
            return physics;
        }
    }
    return nullptr;
}

void GameObject::NotifyCollision(GameObject &other){
    for(unsigned int i = 0; i < components.size();i++){
        if(!components[i]->GetFreeze()){
            components[i]->NotifyCollision(other);
        }
    }
}

