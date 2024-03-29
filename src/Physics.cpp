#include "../include/Physics.h"
#include "../include/Component.h"
#include "../include/Camera.h"
#include "../include/Collision.h"
#include "../include/StageState.h"
#include "../include/WindowEffects.h"

Physics::Physics(GameObject& associated,Vec2 *speed,bool isTile,bool isMob) : Component(associated){
    distground = 0;
    distceiling = 0;
    distright = 0;
    distleft = 0;
    max = 150;
    this->isTile = isTile;
    this->isMob = isMob;
    this->speed = speed;
    this->collider = new Collider(associated);
    associated.AddComponent(collider);
}

Physics::~Physics(){
    collider = nullptr;
    speed = nullptr;
}

void Physics::Update(float dt){
    this->max = 150;
    if(!isTile){
        Vec2 centerofmap = Vec2(Camera::limit.x/2,Camera::limit.y/2);
        while(IsOutofBounds() && !StageState::ChangingMap() && !isMob){
            std::cout << "Out of Bounds" << std::endl;
            Follow(centerofmap,10,1);
            collider->Update(0);
        }

        bool correctdistance = false;
        std::queue<int> adjustindices;
        for(int i = 0;i < TileMap::tiles.size();i ++){
            TileCollider *tilecollider = dynamic_cast<TileCollider*>(TileMap::tiles[i].lock().get());
            bool collided = Collision::IsColliding(collider->box,tilecollider->box,ToPI(associated.angleDeg),0);
            if(collided && tilecollider->moving){
                while(Collision::IsColliding(collider->box,tilecollider->box,ToPI(associated.angleDeg),0)){
                    tilecollider->NotifyMobCollision(associated);
                    collider->Update(0);
                }
            }
            else if(collided && !tilecollider->moving){
                correctdistance = true;
                adjustindices.push(i);
            }
        }

        if(correctdistance){
            for(int i = adjustindices.size();i > 0;i--){
                TileCollider *tilecollider = dynamic_cast<TileCollider*>(TileMap::tiles[adjustindices.front()].lock().get());
                while(Collision::IsColliding(collider->box,tilecollider->box,ToPI(associated.angleDeg),0)){
                    CorrectDistance();
                    collider->Update(0);
                }
                adjustindices.pop();
            }
        }
    }
}

void Physics::Render(){

}
bool Physics::Is(std::string type){
    if(type == "Physics"){
        return true;
    }else{
        return false;
    }
}

void Physics::CorrectDistance(){
    UpdateDists(max);
    if((distground == -max) && (distceiling == -max) && (distright == -max) && (distleft == -max)){
        max *= 2;
    }else{
        std::map<int,float> dists;
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
                associated.box.y += distground;
            }
            if(disttofix[0] == 1){
                associated.box.y -= distceiling;
            }
            if(disttofix[0] == 2){
                associated.box.x += distright;
            }
            if(disttofix[0] == 3){
                associated.box.x -= distleft;
            }
        }
    }
    
}

void Physics::UpdateDists(int max){
    distground = DistanceTo(collider->box,0,1,max);
    distceiling = DistanceTo(collider->box,0,-1,max);
    distright = DistanceTo(collider->box,1,0,max);
    distleft = DistanceTo(collider->box,-1,0,max);
}

void Physics::PrintValues(std::string header){
    #ifdef DEBUG
    UpdateDists();
    Vec2 center = collider->box.GetCenter();

    ENDLINE
    std::cout << header << " -> ";
    std::cout << "xPos: " << center.x << " yPos: " << center.y << std::endl;
    std::cout << header << " -> ";
    std::cout << "dground: "<< distground << std::endl;
    std::cout << header << " -> ";
    std::cout << "dceiling: "<< distceiling << std::endl;
    std::cout << header << " -> ";
    std::cout << "dright: "<< distright << std::endl;
    std::cout << header << " -> ";
    std::cout << "dleft: "<< distleft << std::endl;
    std::cout << header << " -> ";
    std::cout << "speedX: " << speed->x << " speedY: " << speed->y << std::endl;
    #endif
}

float Physics::DistanceTo(Rect box,int xsum,int ysum,int max){
    int dist = 0;

    while(!IsColliding(box,ToPI(associated.angleDeg)) && (dist < max)){
        box.y += ysum;
        box.x += xsum;
        dist ++;
    }
    while(IsColliding(box,ToPI(associated.angleDeg)) && (dist > -max)){
        box.y += -ysum;
        box.x += -xsum;
        dist --;
    }

    return dist;
}

int Physics::SightTo(Vec2 vector,Vec2 vectorTo,int max){
    float distance = vector.GetDistance(vectorTo.x,vectorTo.y);
    float angle = vector.GetAngle(vectorTo.x,vectorTo.y);
    Rect box = GetLineBox(vector,vectorTo);
    if(distance >= max){
        return max;
    }
    if(!IsColliding(box,angle)){
        return distance;
    }else{
        return max;
    }
}

Rect Physics::GetLineBox(Vec2 vector,Vec2 vectorTo,float distance){
    if(distance == 0){
        distance = vector.GetDistance(vectorTo.x,vectorTo.y);
    }
    float angle = vector.GetAngle(vectorTo.x,vectorTo.y);
    Vec2 vectorRot = Vec2(distance,0).GetRotated(angle) + vector;
    return Rect(vectorRot.x - (distance * (((cos(std::fabs(angle))) + 1)/2) ),vectorRot.y + (distance/2 * -sin(angle)),distance,1);
}

bool Physics::IsOutofBounds(bool Completely,Rect box,float angle){
    //If no inputs bases on current collider
    if(box == Rect(0,0,0,0)){
        box = collider->box;
        angle = ToPI(associated.angleDeg);
    }
    Vec2 center( box.GetCenter() );
    Rect map = Rect(0,0,Camera::limit.x,Camera::limit.y);
	Vec2 points[4];

    points[0] = (Vec2(box.x, box.y) - center).GetRotated( angle ) + center;
		
    points[1] = (Vec2(box.x + box.w, box.y) - center).GetRotated( angle ) + center;
		
	points[2] = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated( angle ) + center;
		
	points[3] = (Vec2(box.x, box.y + box.h) - center).GetRotated( angle ) + center;

    if(Completely){
        if(!map.Contains(points[0]) && !map.Contains(points[1]) && !map.Contains(points[2]) && !map.Contains(points[3])){
            return true;
        } 
    }else{
        if(!map.Contains(points[0]) || !map.Contains(points[1]) || !map.Contains(points[2]) || !map.Contains(points[3])){
            return true;
        }  
    }
    return false;
}
bool Physics::IsColliding(Rect box,float angle,bool nooutofbounds,bool markcollision,bool ignoreMob){
    #ifndef DEBUG
        markcollision = false;
    #endif
    if(box == Rect(0,0,0,0)){
        box = collider->box;
        angle = (associated.angleDeg * PI)/180;
    }
    if(IsOutofBounds(false,box,angle) && !StageState::ChangingMap() && !nooutofbounds && !isMob){
        return true;
    }
    for(int i = 0;i < TileMap::tiles.size();i ++){
        TileCollider *tilecollider = dynamic_cast<TileCollider*>(TileMap::tiles[i].lock().get());
        if(Collision::IsColliding(box,tilecollider->box,angle,0,markcollision)){
            if(ignoreMob){
                if(!tilecollider->IsMob()){
                    return true;
                }
            }else{
                return true;
            }
        }
    }
    return false;
}

bool Physics::IsUp(int sum,bool outofbounds){
    return IsColliding(collider->box.Added(0,-sum,0,-collider->box.h),ToPI(associated.angleDeg),!outofbounds);
}

bool Physics::IsGrounded(int sum,bool outofbounds){
    return IsColliding(collider->box.Added(0,sum + collider->box.h,0,-collider->box.h),ToPI(associated.angleDeg),!outofbounds);
}

bool Physics::IsLeft(int sum,bool outofbounds){
    return IsColliding(collider->box.Added(-sum,0,-collider->box.w,0),ToPI(associated.angleDeg),!outofbounds);
}

bool Physics::IsRight(int sum,bool outofbounds){
    return IsColliding(collider->box.Added(sum + collider->box.w,0,-collider->box.w,0),ToPI(associated.angleDeg),!outofbounds);
}

Vec2 Physics::GetCollisionPoint(Vec2 origin,Vec2 dest,Rect colBox,float colBoxAngle){
    Rect box = GetLineBox(origin,dest);
    float angle = origin.GetAngle(dest);

    if(colBox == Rect(0,0,0,0)){
        if(IsColliding(box,angle,true,false,true)){
            float interval = box.w/2;
            box.w -= interval;
            box = GetLineBox(origin,dest,box.w);
            while(interval > 1){
                if(IsColliding(box,angle,true)){
                    box.w -= interval;
                    interval /= 2;
                }
                else{
                    box.w += interval;
                    interval /= 2;
                }
                box = GetLineBox(origin,dest,box.w);
            }
            Vec2 collisionpoint = Vec2(box.w,0).GetRotated(angle) + origin;
            return collisionpoint;
        }else{
            return Vec2(0,0);
        }
    
    }else{
        if(Collision::IsColliding(box,colBox,angle,colBoxAngle)){
            float interval = box.w/2;
            box.w -= interval;
            box = GetLineBox(origin,dest,box.w);
            while(interval > 1){
                if(Collision::IsColliding(box,colBox,angle,colBoxAngle)){
                    box.w -= interval;
                    interval /= 2;
                }
                else{
                    box.w += interval;
                    interval /= 2;
                }
                box = GetLineBox(origin,dest,box.w);
            }
            Vec2 collisionpoint = Vec2(box.w,0).GetRotated(angle) + origin;
            return collisionpoint;
        }else{
            return Vec2(0,0);
        }
    }
}

Vec2 Physics::Follow(Vec2 dest,float constspeed,float dt){
    Vec2 delta;
    float angle = associated.box.GetCenter().GetAngle(dest.x,dest.y);
    speed->x = abs(constspeed * cos(angle));
    speed->y = abs(constspeed * sin(angle));
    if(associated.box.GetCenter().x == dest.x){
        delta.x = 0;
    }
    else if(associated.box.GetCenter().x < dest.x){
        associated.box.x += speed->x * dt;
        delta.x = speed->x * dt;
        if(associated.box.GetCenter().x > dest.x){
            associated.box.x = dest.x - associated.box.w/2;
            delta.x = 0;
        }
    }else{
        associated.box.x -= speed->x * dt;
        delta.x = -(speed->x * dt);
        if(associated.box.GetCenter().x < dest.x){
            associated.box.x = dest.x - associated.box.w/2;
            delta.x = 0;
        }
    }


    if(associated.box.GetCenter().y == dest.y){
        delta.y = 0;
    }
    else if(associated.box.GetCenter().y < dest.y){
        associated.box.y += speed->y * dt;
        delta.y = speed->y * dt;
        if(associated.box.GetCenter().y > dest.y){
            associated.box.y = dest.y - associated.box.h/2;
            delta.y = 0;
        }
    }else{
        associated.box.y -= speed->y * dt;
        delta.y = -(speed->y * dt);
        if(associated.box.GetCenter().y < dest.y){
            associated.box.y = dest.y - associated.box.h/2;
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
    associated.box.Transform(rotationVec.x - associated.box.w/2,rotationVec.y - associated.box.h/2);
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
        if(IsRight() && (speed->x > 0)){
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
        if(IsLeft() && (speed->x < 0)){
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

void Physics::PerformYDeceleration(float despeed,float dt){
    if(speed->y > 0){
        if((speed->y - despeed * dt) < 0){
            speed->y = 0;
        }else{
            speed->y -= despeed * dt;
        }
    }else{
        if((speed->y + despeed * dt) > 0){
            speed->y = 0;
        }else{
            speed->y += despeed * dt;
        }
    }
}

float Physics::PerformXMovement(float dt){
    if(speed->x != 0){
        float modX = 0;
        if((speed->x * dt) < 0){
            modX = speed->x * dt;
        }
        if(IsColliding(collider->box.Added(modX,0,std::fabs(speed->x * dt),-1),ToPI(associated.angleDeg))){
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
            associated.box.x += speed->x * dt;
            return speed->x * dt;
        }
    }
}

float Physics::PerformYMovement(float dt){
    if(speed->y != 0){
        float modY = 0;
        float dif = speed->y/2;
        if((speed->y * dt) < 0){
            modY = speed->y * dt;
        }
        if(IsColliding(collider->box.Added(0,modY,0,std::fabs(speed->y * dt)),ToPI(associated.angleDeg))){
            if(IsGrounded() && (speed->y > 0)){
                speed->y = 0;
                dif = 0;
                return 0;
            }
            if(IsUp() && (speed->y < 0)){
                speed->y = 0;
                dif = 0;
                return 0;
            }
            while(dif > 1){
                if((speed->y * dt) < 0){
                    modY = speed->y * dt;
                }
                if(IsColliding(collider->box.Added(0,modY,0,std::fabs(speed->y * dt)),ToPI(associated.angleDeg))){
                    speed->y - dif;
                    dif /= 2;
                }else{
                    speed->y + dif;
                    dif /= 2;
                }
            }
        }
        else{
            associated.box.y += speed->y * dt;
            return speed->y * dt;
        }
    }
}

void Physics::PerformGravity(float gravspeed,float dt){
    if(!IsGrounded()){
        if(IsRight()){
            speed->y += gravspeed * dt;
        }
        else if(IsLeft()){
            speed->y += gravspeed * dt;
        }else{
            float DistToGround = DistanceTo(collider->box,0,1,30);
            if((DistToGround <= 20) && (speed->y >= 0)){
                collider->box.y += DistToGround;
                collider->UpdateAssociated();
                if(IsGrounded()){
                    speed->y = 0;
                }
            }
            else{
                speed->y += gravspeed * dt;
            }
        }
    }
}

void Physics::KnockBack(Rect hitbox,Vec2 knockback,bool sum,Vec2 max){
    if(!sum){
        if((collider->box.x + collider->box.w/2) <= (hitbox.x + hitbox.w/2)){
            speed->x = -knockback.x;
        }else{
            speed->x = knockback.x;
        }
        speed->y = -knockback.y;
    }else{
        if((collider->box.x + collider->box.w/2) <= (hitbox.x + hitbox.w/2)){
            PerformXAcceleration(false,knockback.x,max.x,0,1);
        }else{
            PerformXAcceleration(true,knockback.x,max.x,0,1);
        }
        PerformYAcceleration(false,knockback.y,1);
    }
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