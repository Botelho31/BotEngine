#include "../include/MovingTile.h"
#include "../include/TileCollider.h"
#include "../include/Collision.h"

MovingTile::MovingTile(GameObject& associated,float speed,Vec2 start,Vec2 dest,bool circular) : Component(associated){
    this->tilesprite = new Sprite(associated,"assets/img/penguin.png");
    associated.AddComponent(tilesprite);
    this->associated.box.x = start.x - associated.box.w/2;
    this->associated.box.y = start.y - associated.box.h/2;

    this->physics = new Physics(&associated,&this->speed,true);
    physics->SetCollider(1,2,0,-associated.box.h/2);

    this->deltamov = Vec2(0,0);
    this->constspeed = speed;
    this->speed =  Vec2(speed,speed);
    this->start = start;
    this->dest = dest;
    this->going = true;
    this->circular = circular;

    TileCollider *tilecollider = new TileCollider(associated,associated.box,true);
    std::weak_ptr<Component> weakptr = associated.AddComponent(tilecollider);
    if(!weakptr.expired()){
        TileMap::tiles.push_back(weakptr);
    }

    Vec2 halfway = Vec2((start.x + dest.x)/2,(start.y + dest.y)/2);
    this->angle = halfway.GetAngle(start.x,start.y);
    if(this->angle < 0){
        this->angle = 2*PI + this->angle;
    }
}

MovingTile::~MovingTile(){
    delete physics;
    tilesprite = nullptr;
}

void MovingTile::Update(float dt){
    physics->Update();
    
    if(circular){
        float newangle = 0;
        Vec2 halfway = Vec2((start.x + dest.x)/2,(start.y + dest.y)/2);
        float radius = halfway.GetDistance(start.x,start.y);
        if(going){
            newangle = physics->Rotate(start,dest,this->angle,constspeed,dt);
        }else{
            newangle = physics->Rotate(dest,start,this->angle,-constspeed,dt);
        }
        float difangle = this->angle - newangle;
        this->angle = newangle;
        if(std::fabs(difangle) > 6){
            difangle = 0.00000001;
        }
        deltamov = Vec2(std::fabs(sin(this->angle) * (difangle*radius)),std::fabs(cos(this->angle) * (difangle*radius)));
        if(this->angle < PI/2){
            deltamov.y = -deltamov.y;
        }
        else if(this->angle < PI){
        }
        else if(this->angle < ((3*PI)/2)){
            deltamov.x = -deltamov.x;
        }
        else if(this->angle < 2*PI){
            deltamov.x = -deltamov.x;
            deltamov.y = -deltamov.y;
        }
        if(difangle < 0){
            deltamov.x = -deltamov.x;
            deltamov.y = -deltamov.y;
        }
    }else{
        if(going){
            deltamov = physics->Follow(dest,constspeed,dt);
        }else{
            deltamov = physics->Follow(start,constspeed,dt);
        }
    }

    if(deltamov == Vec2(0,0)){
        InvertDirection();
    }
}

void MovingTile::InvertDirection(){
    if(going){
        going = false;
    }else{
        going = true;
    }
}

void MovingTile::Render(){

}

void MovingTile::NotifyCollision(GameObject& other){
    Physics *physics1 = other.GetPhysics();
    if(physics1){
        bool invert = false;
        Collider *collider1 = physics1->GetCollider();
        
        if(deltamov.x != 0){
            Rect movedX = collider1->box.Added(deltamov.x,0);
            if(!physics1->IsColliding(movedX,ToPI(other.angleDeg))){
                other.box.x += deltamov.x;
            }
        }
            // else{
                // if(Collision::IsColliding(collider1->box,associated.box,other.angleDeg,associated.angleDeg)){
                //     invert = true;
                // }
        //         while(physics1->IsColliding(movedX,other.angleDeg) && (deltamov.x != 0)){
        //             deltamov.x /= 2;
        //             if(deltamov.x < 1){
        //                 deltamov.x = 0;
        //             }
        //             movedX = collider1->box.Added(deltamov.x,0);
        //         }
        //         other.box.x += deltamov.x;
        //     }
        // }
        
        if(deltamov.y != 0){
            Rect movedY = collider1->box.Added(0,deltamov.y);
            if(!physics1->IsColliding(movedY,ToPI(other.angleDeg))){
                other.box.y += deltamov.y;
            }else{
                while(physics1->IsColliding(movedY,ToPI(other.angleDeg)) && (deltamov.y != 0)){
                    deltamov.y /= 2;
                    if(deltamov.y < 1){
                        deltamov.y = 0;
                    }
                    movedY = collider1->box.Added(0,deltamov.y);
                }
                std::cout << deltamov.y << std::endl;
                other.box.y += deltamov.y;
                
            }
        }
        // if(invert){
        //     InvertDirection();
        // }
    }
}

bool MovingTile::Is(std::string type){
    if(type == "MovingTile"){
        return true;
    }else{
        return false;
    }
}