#include "../include/Physics.h"
#include "../include/Component.h"
#include "../include/Camera.h"


Physics::Physics(GameObject* associated,Vec2 *speed,bool isTile) : associated(associated){
    distground = 0;
    distceiling = 0;
    distright = 0;
    distleft = 0;
    max = 150;
    this->isTile = isTile;
    this->speed = speed;
    this->collider = new Collider(*associated);
    associated->AddComponent(collider);
}

Physics::~Physics(){
    associated = nullptr;
    collider = nullptr;
    speed = nullptr;
}

void Physics::Update(Rect collider,int max){
    this->max = max;
    Vec2 BoxCollider[] = {   Vec2(collider.x,collider.y),
                        Vec2(collider.x + collider.w,collider.y),
                        Vec2(collider.x,collider.y + collider.h),
                        Vec2(collider.x + collider.w,collider.y + collider.h)
    };
    distground = DistanceTo( BoxCollider[2], BoxCollider[3],0,1,max);
    distceiling = DistanceTo( BoxCollider[0], BoxCollider[1],0,-1,max);
    distright = DistanceTo( BoxCollider[1], BoxCollider[3],1,0,max);
    distleft = DistanceTo( BoxCollider[0], BoxCollider[2],-1,0,max);
}

void Physics::CorrectDistance(){
    if((distground == -max) && (distceiling == -max) && (distright == -max) && (distleft == -max)){
        Update(collider->box,Camera::limit.x);
        std::cout << "Out of Bounds" << std::endl;
    }
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
    int x,y;
    x = (vector2.x - vector1.x)/10;
    y = (vector2.y - vector1.y)/10;
    for(int i = 0;i < 10;i++){
        if(!CanMove(vector1)){
            return false;
        }
        vector1.x += x;
        vector1.y += y;
    }
    return true;
}

bool Physics::CanMove(Vec2 vector){
    TileMap *tilemap = Game::GetInstance().GetCurrentState().GetTileMap();
    if(tilemap->AtLocation(vector.x,vector.y,isTile) > -1){
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

Vec2 Physics::GetCollisionPoint(Rect hitbox){
    Vec2 hitboxcenter = hitbox.GetCenter();
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
    Vec2 error = Vec2(0,0);
    if((!disttofix.empty()) && (dists[disttofix[0]] > -max)){
        Vec2 pointofcollision = Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
        if(disttofix[0] == 0){
            pointofcollision.y = collider->box.y + collider->box.h + distground;
            if(pointofcollision.y >= hitboxcenter.y){
                pointofcollision.y = collider->box.y + collider->box.h - distground;
            }
        }
        if(disttofix[0] == 1){
            pointofcollision.y = collider->box.y - distceiling;
            if(pointofcollision.y <= hitboxcenter.y){
                pointofcollision.y = collider->box.y + distceiling;
            }
        }
        if(disttofix[0] == 2){
            pointofcollision.x = collider->box.x + collider->box.w + distright;
            if(pointofcollision.x <= hitboxcenter.x){
                pointofcollision.x = collider->box.x + collider->box.w - distright;
            }
        }
        if(disttofix[0] == 3){
            pointofcollision.x = collider->box.x - distleft;
            if(pointofcollision.x >= hitboxcenter.x){
                pointofcollision.x = collider->box.x + distleft;
            }
        }
        return pointofcollision;
    }
    return error;
}

Vec2 Physics::Follow(Vec2 dest,float dt){
    Vec2 delta;
    if(associated->box.GetCenter().x == dest.x){
        delta.x = 0;
    }
    else if(associated->box.GetCenter().x < dest.x){
        associated->box.x += speed->x * dt;
        delta.x = speed->x * dt;
        if(associated->box.GetCenter().x > dest.x){
            associated->box.x = dest.x - associated->box.w/2;
            delta.x = 0;
        }
    }else{
        associated->box.x -= speed->x * dt;
        delta.x = -(speed->x * dt);
        if(associated->box.GetCenter().x < dest.x){
            associated->box.x = dest.x - associated->box.w/2;
            delta.x = 0;
        }
    }


    if(associated->box.GetCenter().y == dest.y){
        delta.y = 0;
    }
    else if(associated->box.GetCenter().y < dest.y){
        associated->box.y += speed->y * dt;
        delta.y = speed->y * dt;
        if(associated->box.GetCenter().y > dest.y){
            associated->box.y = dest.y - associated->box.h/2;
            delta.y = 0;
        }
    }else{
        associated->box.y -= speed->y * dt;
        delta.y = -(speed->y * dt);
        if(associated->box.GetCenter().y < dest.y){
            associated->box.y = dest.y - associated->box.h/2;
            delta.y = 0;
        }
    }
    return delta;
}

void Physics::PerformXAcceleration(bool increaseX,float aspeed,float maxspeed,float despeed,float dt){
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

void Physics::PerformXDeceleration(float despeed,float dt){
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

float Physics::PerformXMovement(float dt){
    if(((distright - (speed->x * dt)) < 0) && (distright >= 0)){
        associated->box.x += distright;
        speed->x = 0;
        return distright;
    }else if(((distleft + (speed->x * dt)) < 0) && (distleft >= 0)){
        associated->box.x -= distleft;
        speed->x = 0;
        return -distleft;
    }else if((distright <= 0) && (speed->x > 0)){
        speed->x = 0;
        return 0;
    }else if((distleft <= 0) && (speed->x < 0)){
        speed->x = 0;
        return 0;
    }else{
        associated->box.x += speed->x * dt;
        return speed->x * dt;
    }
}

float Physics::PerformYMovement(float dt){
    if((((distground - (speed->y * dt)) < 0) && (speed->y > 0)) && (distground >= 0)){
        associated->box.y += distground;
        speed->y = 0;
        return distground;
    }
    else if(((distceiling + (speed->y * dt) < 0) && (speed->y < 0)) && (distceiling >= 0)){
        associated->box.y -= distceiling;
        speed->y = 0;
        return -distceiling;
    }else{
        associated->box.y += speed->y * dt;
        return speed->y * dt;
    }
}

void Physics::PerformGravity(float gravspeed,float dt){
    if(!IsGrounded()){
        if(abs(distground) < 3){
            associated->box.y += distground;
        }
        speed->y += gravspeed * dt;
    }
}

void Physics::KnockBack(Rect hitbox,Vec2 knockback){
    if((collider->box.x + collider->box.w/2) <= (hitbox.x + hitbox.w/2)){
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