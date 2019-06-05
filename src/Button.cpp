#include "../include/Button.h"
#include "../include/MousePointer.h"
#include "../include/Collider.h"

Button::Button(GameObject& associated,Vec2 center,std::string normalfile,std::string highlightedfile,std::string selectedfile) : Component(associated){
    buttonsprite = new Sprite(associated,normalfile);
    associated.AddComponent(buttonsprite);
    this->normalfile = normalfile;
    this->highlightedfile = highlightedfile;
    this->selectedfile = selectedfile;
    state = NORMAL;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    this->center = center;
    associated.box.SetCenter(center);
}

Button::~Button(){

}

void Button::Update(float dt){
    if((!colliding) && (state != NORMAL)){
        if(state == SELECTED){
            SetSprite(normalfile);
            associated.box.SetCenter(center);
        }else{
            SetSprite(normalfile);
        }
        state = NORMAL;
    }

    colliding = false;
}

void Button::Render(){

}

bool Button::isSelected(){
    if(state == SELECTED){
        return true;
    }else{
        return false;
    }
}

void Button::Reset(){
    state = NORMAL;
    colliding = false;
    SetSprite(normalfile);
}

void Button::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    buttonsprite->SetFrameCount(framecount);
    buttonsprite->SetFrameTime(frametime);
    buttonsprite->SetRepeat(repeat);
    buttonsprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
}

bool Button::Is(std::string type){
    if(type == "Button"){
        return true;
    }else{
        return false;
    }
}

void Button::NotifyCollision(GameObject& other){
    Component *component = other.GetComponent("MousePointer");
    if(component){
        colliding = true;
        MousePointer *mousepointer = dynamic_cast<MousePointer*>(component);
        if(mousepointer->IsPressed()){
            if(state != SELECTED){
                state = SELECTED;
                SetSprite(selectedfile);
            }
        }else{
            if(state != HIGHLIGHTED){
                state = HIGHLIGHTED;
                SetSprite(highlightedfile);
            }
        }
    }
}