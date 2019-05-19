#include "../include/MovingTile.h"
#include "../include/Collision.h"

MovingTile::MovingTile(GameObject& associated,float speed,Vec2 start,Vec2 dest,bool circular) : Component(associated){
    this->tilesprite = new Sprite(associated,"assets/img/movingtile1.png");
    associated.AddComponent(tilesprite);
    this->associated.box.x = start.x - associated.box.w/2;
    this->associated.box.y = start.y - associated.box.h/2;

    this->physics = new Physics(associated,&this->speed,true);
    associated.AddComponent(physics);
    physics->SetCollider(0.95,2,0,-associated.box.h/2);

    this->deltamov = Vec2(0,0);
    this->constspeed = speed;
    this->speed =  Vec2(speed,speed);
    this->start = start;
    this->dest = dest;
    this->going = true;
    this->circular = circular;

    this->tilecollider = new TileCollider(associated,associated.box,true);
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
    physics = nullptr;
    tilesprite = nullptr;
}

void MovingTile::Update(float dt){
    // physics->Update(dt);
    
    if(tilecollider->pressing){
        tilecollider->pressing = false;
        InvertDirection();
    }

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

    for(int i = 0;i < movingList.size();i++){
        if(movingList.front().expired()){
            movingList.pop();
        }else{
            MoveObject(*movingList.front().lock().get(),deltamov);
            movingList.pop();
        }
    }

    if(deltamov == Vec2(0,0)){
        InvertDirection();
    }
    
    tilecollider->Update(0);
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
        if(!other.GetComponent("HitBox")){
            std::weak_ptr<GameObject> otherweakptr = Game::GetInstance().GetCurrentState().GetObjectPtr(&other);
            movingList.push(otherweakptr);
        }
    }
}

void MovingTile::MoveObject(GameObject& other,Vec2 deltamov){
        Physics *physics1 = other.GetPhysics();
        Collider *collider1 = physics1->GetCollider();

        if(deltamov.x != 0){
            Rect movedX = collider1->box.Added(deltamov.x,0);
            if(!physics1->IsColliding(movedX,ToPI(other.angleDeg))){
                other.box.x += deltamov.x;
            }else{
                float interval = deltamov.x/2;
                while(interval > 0.01){
                    if(physics1->IsColliding(movedX,ToPI(other.angleDeg))){
                        deltamov.x -= interval;
                        interval /= 2;
                    }else{
                        deltamov.x += interval;
                        interval /= 2;
                    }
                    movedX = collider1->box.Added(deltamov.x,deltamov.y);
                }
                other.box.x += deltamov.x; 
            }
        }
        
        if(deltamov.y != 0){
            Rect movedY = collider1->box.Added(0,deltamov.y);
            if(!physics1->IsColliding(movedY,ToPI(other.angleDeg))){
                other.box.y += deltamov.y;
            }else{
                float interval = deltamov.y/2;
                while(interval > 0.01){
                    if(physics1->IsColliding(movedY,ToPI(other.angleDeg))){
                        deltamov.y -= interval;
                        interval /= 2;
                    }else{
                        deltamov.y += interval;
                        interval /= 2;
                    }
                    movedY = collider1->box.Added(deltamov.x,deltamov.y);
                }
                other.box.y += deltamov.y;                
            }
        }
}

bool MovingTile::Is(std::string type){
    if(type == "MovingTile"){
        return true;
    }else{
        return false;
    }
}