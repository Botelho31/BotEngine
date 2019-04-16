#include "../include/Physics.h"
#include "../include/Component.h"


Physics::Physics(GameObject* associated) : associated(associated){
    distground = 0;
    distceiling = 0;
    distright = 0;
    distleft = 0;
    this->collider = new Collider(*associated);
    associated->AddComponent(collider);
}

Physics::~Physics(){
    associated = nullptr;
    collider = nullptr;
}

void Physics::Update(Rect collider){
    Vec2 BoxCollider[] = {   Vec2(collider.x,collider.y),
                        Vec2(collider.x + collider.w,collider.y),
                        Vec2(collider.x,collider.y + collider.h),
                        Vec2(collider.x + collider.w,collider.y + collider.h)
    };
    distground = DistanceTo( BoxCollider[2], BoxCollider[3],0,1);
    distceiling = DistanceTo( BoxCollider[0], BoxCollider[1],0,-1);
    distright = DistanceTo( BoxCollider[1], BoxCollider[3],1,0);
    distleft = DistanceTo( BoxCollider[0], BoxCollider[2],-1,0);
}

void Physics::CorrectDistance(){
    // if((distground == -150) && (distceiling == -150) && (distright == -150) && (distleft == -150)){

    // }
    std::map<int,int> dists;
    dists.insert({0,distground});
    dists.insert({1,distceiling});
    dists.insert({2,distright});
    dists.insert({3,distleft});
    std::deque<int> disttofix;
    for(int i = 0;i < 4;i++){
        if(dists[i] < 0){
            disttofix.push_front(i);
        }
    }
    bool inserted = true;
    while(inserted){
        inserted = false;
        for(unsigned int i = 0;i < disttofix.size();i++){
            if(i != (disttofix.size() -1)){
                if(dists[disttofix[i]] < dists[disttofix[i + 1]]){
                    int a = disttofix[i];
                    disttofix[i] = disttofix[i + 1];
                    disttofix[i + 1] = a;
                    inserted = true;
                }
            }
        }
    }
    // for(int i = 0;i < disttofix.size();i++){
    //     std::cout << i << " " << disttofix[i] << " " << dists[disttofix[i]] << std::endl;   
    // }
    if(!disttofix.empty()){
        if(disttofix[0] == 0){
            associated->box.y += distground;
        }
        if(disttofix[0] == 1){
            associated->box.y -= distceiling;
        }
        if(disttofix[0] == 2){
            associated->box.x += distright;
        }
        if(disttofix[0] == 3){
            associated->box.x -= distleft;
        }
    }

}


int Physics::DistanceTo(Vec2 vector1,Vec2 vector2,int xsum,int ysum,int max){
    int distance = 0;
    while(CanMove(vector1,vector2) && (distance < max)){
        vector1.y += ysum;
        vector2.y += ysum;
        vector1.x += xsum;
        vector2.x += xsum;
        distance ++;
    }
    while(!CanMove(vector1,vector2) && (distance > -max)){
        vector1.y += -ysum;
        vector2.y += -ysum;
        vector1.x += -xsum;
        vector2.x += -xsum;
        distance --;
    }
    return distance;
}

int Physics::DistanceTo(Vec2 vector,Vec2 vectorTo,int max){
    float distance = 0;
    float sum = 0;
    Vec2 original = vector;
    float angle = (vectorTo.y - vector.y)/(vectorTo.x - vector.x);
    float b = vector.y - (angle * vector.x);
    if(vector.x > vectorTo.x){
        sum = -1.0;
    }else{
        sum = 1.0;
    }
    while(CanMove(vector) && (distance <= max) && (((sum > 0) && (vector.x <= vectorTo.x)) || ((sum < 0) && (vector.x >= vectorTo.x)) )){
        vector.x += sum;
        vector.y = (vector.x * angle) + b;
        distance  =  vector.GetDistance(original.x,original.y);
    }
    if(!CanMove(vector)){
        distance = max;
    }
    
    return distance;
}

bool Physics::CanMove(Vec2 vector1,Vec2 vector2){
    TileMap *tilemap = Game::GetInstance().GetCurrentState().GetTileMap();
    int x,y;
    x = (vector2.x - vector1.x)/10;
    y = (vector2.y - vector1.y)/10;
    for(int i = 0;i < 10;i++){
        if(tilemap->AtLocation(vector1.x,vector1.y) > -1){
            return false;
        }
        vector1.x += x;
        vector1.y += y;
    }
    return true;
}

bool Physics::CanMove(Vec2 vector){
    TileMap *tilemap = Game::GetInstance().GetCurrentState().GetTileMap();
    if(tilemap->AtLocation(vector.x,vector.y) > -1){
        return false;
    }else{
        return true;
    }
}

bool Physics::IsGrounded(){
    if(distground <= 0){
        return true;
    }else{
        return false;
    }
}

void Physics::PerformXAcceleration(Vec2 *speed,bool increaseX,float aspeed,float maxspeed,float despeed,float dt){
    if(increaseX){
        if(speed->x >= 0){
            if((speed->x + aspeed*dt) > maxspeed){
                speed->x = maxspeed;
            }else{
                speed->x += aspeed * dt;
            }
        }else{
           speed->x += aspeed * dt + despeed * dt; 
        }
    }else{
        if(speed->x <= 0){
            if(abs(speed->x - aspeed*dt) > maxspeed){
                speed->x = -maxspeed;
            }else{
                speed->x -= aspeed * dt;
            }
        }else{
            speed->x -= aspeed *dt + despeed *dt;
        }
    }
}

void Physics::PerformXDeceleration(Vec2 *speed,float despeed,float dt){
    if(speed->x > 0){
        if((speed->x - despeed * dt) < 0){
            speed->x = 0;
        }else{
            speed->x -= despeed * dt;
        }
    }else{
        if((speed->x + despeed * dt) > 0){
            speed->x = 0;
        }else{
            speed->x += despeed * dt;
        }
    }
}

void Physics::PerformXMovement(Vec2 *speed,float dt){
    if((distright - (speed->x * dt)) < 0){
        associated->box.x += distright;
        speed->x = 0;
    }else if((distleft + (speed->x * dt)) < 0){
        associated->box.x -= distleft;
        speed->x = 0;
    }else if((distright < 0) && (speed->x > 0)){
        speed->x = 0;
    }else if((distleft < 0) && (speed->x < 0)){
        speed->x = 0;
    }else{
        associated->box.x += speed->x * dt;
    }
}

void Physics::PerformYMovement(Vec2 *speed,float dt){
    if(((distground - (speed->y * dt)) < 0) && (speed->y > 0)){
        associated->box.y += distground;
    }
    else if((distceiling + (speed->y * dt) < 0) && (speed->y < 0)){
        associated->box.y -= distceiling;
        speed->y = 0;
    } 
    else{
        associated->box.y += speed->y * dt;
    }
}

void Physics::KnockBack(Rect hitbox,Vec2 *speed,Vec2 knockback){
    if(collider->box.x < hitbox.x){
        speed->x = -knockback.x;
    }else{
        speed->x = knockback.x;
    }
    speed->y = -knockback.y;
}

Collider* Physics::GetCollider(){
    if(collider){
        return collider;
    }else{
        return nullptr;
    }
}

void Physics::SetCollider(float scaleX,float scaleY,float offsetX,float offsetY){
    if(collider){
        collider->SetScale(Vec2(scaleX,scaleY));
        collider->SetOffSet(Vec2(offsetX,offsetY));
    }
}