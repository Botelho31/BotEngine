#include "../include/Bullet.h"
#include "../include/Sprite.h"

Bullet::Bullet(GameObject& associated,float angle,float speed,int damage,float maxDistance,std::string sprite,int frameCount,float frameTime) : 
    Component(associated){
        Sprite *bullet = new Sprite(associated,sprite,frameCount,frameTime);
        associated.AddComponent(bullet);
        associated.angleDeg = -((angle * 180)/PI);
        this->speed.x = speed;
        this->speed.y = speed;
        this->distanceLeft = maxDistance;
        this->damage = damage;
        this->angle = angle;
}

void Bullet::Update(float dt){
    associated.box.x += (cos(angle) * speed.x) * dt;
    associated.box.y += ((-sin(angle)) * speed.y) * dt;
    distanceLeft -= fabs((((-sin(angle)) * speed.y) * dt))  + fabs(((cos(angle) * speed.x) * dt));
    if(distanceLeft <= 0){
        associated.RequestDelete();
    }
}

void Bullet::Render(){

}

bool Bullet::Is(std::string type){
    if(type == "Bullet"){
        return true;
    }else{
        return false;
    }
}

int Bullet::GetDamage(){
    return damage;
}

