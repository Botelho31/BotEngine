#include "../include/Physics.h"
#include "../include/Component.h"
#include "../include/Camera.h"
#include "../include/Collision.h"
#include "../include/StageState.h"

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

void Physics::Update(int max){
    this->max = max;
    collider->Update(0);
    if(!isTile){
        if((((collider->box.x + collider->box.w) > Camera::limit.x) || (collider->box.x < 0) || (collider->box.y < 0) || ((collider->box.y + collider->box.h) > Camera::limit.y)) && !StageState::ChangingMap()){
            std::cout << "Out of Bounds" << std::endl;
            if((collider->box.x + collider->box.w) > Camera::limit.x){
                associated->box.x += Camera::limit.x - (collider->box.x + collider->box.w);
            }
            if(collider->box.x < 0){
                associated->box.x += collider->box.x;
            }
            if((collider->box.y + collider->box.h) > Camera::limit.y){
                associated->box.y += Camera::limit.y - (collider->box.y + collider->box.h);
            }
            if(collider->box.y < 0){
                associated->box.y = collider->box.y;
            }
        }
        for(int i = 0;i < TileMap::tiles.size();i ++){
            TileCollider *tilecollider = dynamic_cast<TileCollider*>(TileMap::tiles[i].lock().get());
            Collider *collider1 = GetCollider();
            while(Collision::IsColliding(collider1->box,tilecollider->box,associated->angleDeg,0)){
                CorrectDistance();
                collider1->Update(0);
            }
        }
    }
}

void Physics::UpdateDists(){
    distground = DistanceTo(collider->box,0,1,max);
    distceiling = DistanceTo(collider->box,0,-1,max);
    distright = DistanceTo(collider->box,1,0,max);
    distleft = DistanceTo(collider->box,-1,0,max);
}

void Physics::CorrectDistance(){
    UpdateDists();
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
    #ifdef DEBUG
    // for(int i = 0;i < disttofix.size();i++){
    //     std::cout << i << " " << disttofix[i] << " " << dists[disttofix[i]] << std::endl;   
    // }
    #endif
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

int Physics::DistanceTo(Rect box,int xsum,int ysum,int max){
    int distance = 0;
    while(!IsColliding(box,associated->angleDeg) && (distance < max)){
        box.y += ysum;
        box.x += xsum;
        distance ++;
    }
    while(IsColliding(box,associated->angleDeg) && (distance > -max)){
        box.y += -ysum;
        box.x += -xsum;
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

bool Physics::CanMove(Vec2 vector){
    TileMap *tilemap = Game::GetInstance().GetCurrentState().GetTileMap();
    if(tilemap->AtLocation(vector.x,vector.y) > -1){
        return false;
    }else{
        return true;
    }
}

bool Physics::IsColliding(Rect box,float angle){
    for(int i = 0;i < TileMap::tiles.size();i ++){
        TileCollider *tilecollider = dynamic_cast<TileCollider*>(TileMap::tiles[i].lock().get());
        if(Collision::IsColliding(box,tilecollider->box,angle,0)){
            return true;
        }
    }
    return false;
}

bool Physics::IsUp(int sum){
    return IsColliding(collider->box.Added(0,-sum,0,-collider->box.h),associated->angleDeg);
}

bool Physics::IsGrounded(int sum){
    return IsColliding(collider->box.Added(0,sum + collider->box.h,0,-collider->box.h),associated->angleDeg);
}

bool Physics::IsLeft(int sum){
    return IsColliding(collider->box.Added(-sum,0,-collider->box.w,0),associated->angleDeg);
}

bool Physics::IsRight(int sum){
    return IsColliding(collider->box.Added(sum + collider->box.w,0,-collider->box.w,0),associated->angleDeg);
}


Vec2 Physics::GetCollisionPoint(Rect hitbox){
    Vec2 hitboxcenter = hitbox.GetCenter();
    UpdateDists();
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

Vec2 Physics::Follow(Vec2 dest,float constspeed,float dt){
    Vec2 delta;
    float angle = associated->box.GetCenter().GetAngle(dest.x,dest.y);
    speed->x = abs(constspeed * cos(angle));
    speed->y = abs(constspeed * sin(angle));
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

float Physics::Rotate(Vec2 start,Vec2 dest,float angle,float constspeed,float dt){
    Vec2 halfway = Vec2((start.x + dest.x)/2,(start.y + dest.y)/2);
    float startangle = halfway.GetAngle(start.x,start.y);
    float finalangle = halfway.GetAngle(dest.x,dest.y);
    float anglespeed = constspeed/halfway.GetDistance(start.x,start.y);
    bool RotationMade = false;
    //Modifies Start and Final according to rotations
    if(startangle < 0){
        startangle = 2*PI + startangle;
    }
    if(finalangle < 0){
        finalangle = 2*PI + finalangle;
    }
    //Checks if rotation has been made
    if((startangle > angle) && (anglespeed >= 0)){
        RotationMade = true;
    }
    else if((startangle < angle) && (anglespeed >= 0)){
        RotationMade = false;
    }
    if((startangle < angle) && (anglespeed <= 0)){
        RotationMade = true;
    }
    else if((startangle < angle) && (anglespeed <= 0)){
        RotationMade = false;
    }

    angle += anglespeed * dt;
    if(anglespeed <= 0){    //Anti-Horario
        if(finalangle <= startangle){
            if(angle <= finalangle){
                angle = finalangle;
            }
        }else{
            if((angle <= finalangle) && (RotationMade)){
                angle = finalangle;
            }
        }
    }else{                  //Horario
        if(finalangle >= startangle){
            if(angle >= finalangle){
                angle = finalangle;
            }
        }else{
            if((angle >= finalangle) && (RotationMade)){
                angle = finalangle;
            }
        }
    }

    //Corrects angle according to rotation
    if(angle < 0){
        angle = 2*PI + angle;
    }
    if(std::fabs(angle * 180/PI) >= 360){
        angle = 0;
    }

    // std::cout << anglespeed << std::endl;
    // std::cout << "degrees: " << angle * 180/PI << std::endl;
    // std::cout << "angle: " << angle << "finalangle: " << finalangle << std::endl;
    Vec2 rotationVec = Vec2(halfway.GetDistance(start.x,start.y),0).GetRotated(angle) + halfway;
    associated->box.Transform(rotationVec.x - associated->box.w/2,rotationVec.y - associated->box.h/2);
    return angle;
}

void Physics::PerformYAcceleration(bool increaseY,float aspeed,float dt){
    if(increaseY){
        speed->y += aspeed * dt;
        if(IsGrounded()){
            speed->y = 0;
        }
    }else{
        speed->y -= aspeed * dt;
        if(IsUp()){
            speed->x = 0;
        }
    }
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
        if(IsRight()){
            speed->x = 0;
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
        if(IsLeft()){
            speed->x = 0;
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
    float modX = 0;
    if((speed->x * dt) < 0){
        modX = speed->x * dt;
    }
    if(IsColliding(collider->box.Added(modX,0,std::fabs(speed->x * dt),0),associated->angleDeg)){
        speed->x = speed->x/2;
        if(IsRight() && (speed->x > 0)){
            speed->x = 0;
            return 0;
        }
        if(IsLeft() && (speed->x < 0)){
            speed->x = 0;
            return 0;
        }
        else{
            PerformXMovement(dt);
        }
    }
    else{
        associated->box.x += speed->x * dt;
        return speed->x * dt;
    }
}

float Physics::PerformYMovement(float dt){
    float modY = 0;
    if((speed->y * dt) < 0){
        modY = speed->y * dt;
    }
    if(IsColliding(collider->box.Added(0,modY,0,std::fabs(speed->y * dt)),associated->angleDeg)){
        speed->y = speed->y/2;
        if(IsGrounded() && (speed->y > 0)){
            speed->y = 0;
            return 0;
        }
        if(IsUp() && (speed->y < 0)){
            speed->y = 0;
            return 0;
        }
        else{
            PerformYMovement(dt);
        }
    }
    else{
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