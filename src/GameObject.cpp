#include "../include/GameObject.h"
#include "../include/Component.h"
#include "../include/Physics.h"


GameObject::GameObject(){
    this->isDead = false;
    this->started = false;
    this->angleDeg = 0;
    this->renderAfterForeGround = false;
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

std::weak_ptr<Component> GameObject::AddComponent(Component* cpt,int place){
    std::shared_ptr<Component> component(cpt);
    if((place == 0) || (place >= components.size())){
        components.emplace_back(component);
    }else{
        components.insert(components.begin() + place,component);
    }
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

void GameObject::ChangeComponentOrder(std::string type,std::string type2){
    int comp1 = -1;
    int comp2 = -1;
    Component *exchange1,*exchange2;
    for(unsigned int i = 0; i < components.size();i++){
        if(components[i]->Is(type)){
            comp1 = i;
            exchange1 = components[comp1].get();
        }
        else if(components[i]->Is(type2)){
            comp2 = i;
            exchange2 = components[comp2].get();
        }
    }
    if((comp1 >= 0) && (comp2 >= 0) && (comp1 != comp2)){
        if(comp1 > comp2){
            int trade;
            trade = comp1;
            comp1 = comp2;
            comp2 = trade;
        }
        std::iter_swap(components.begin() + comp1,components.begin() + comp2);
    }
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

