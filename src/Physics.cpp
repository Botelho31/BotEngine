#include "../include/Physics.h"
#include "../include/Component.h"
#include "../include/Camera.h"
#include "../include/Collision.h"
#include "../include/StageState.h"
#include "../include/WindowEffects.h"

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
        Vec2 centerofmap = Vec2(Camera::limit.x/2,Camera::limit.y/2);
        while((IsOutofBounds(collider->box,(associated->angleDeg * PI) /180)) && !StageState::ChangingMap()){
            std::cout << "Out of Bounds" << std::endl;
            Follow(centerofmap,10,1);
            collider->Update(0);
        }
        for(int i = 0;i < TileMap::tiles.size();i ++){
            TileCollider *tilecollider = dynamic_cast<TileCollider*>(TileMap::tiles[i].lock().get());
            while(Collision::IsColliding(collider->box,tilecollider->box,(associated->angleDeg * PI) /180,0)){
                if(tilecollider->moving){
                    tilecollider->NotifyMobCollision(*associated);
                    collider->Update(0);
                }else{
                    CorrectDistance();
                    collider->Update(0);
                }
            }
        }
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
    
}

void Physics::UpdateDists(int max){
    // std::cout << "Update dists" << associated->box.x << associated->box.y << std::endl;
    distground = DistanceTo(collider->box,0,1,PI/2,max);
    distceiling = DistanceTo(collider->box,0,-1,-PI/2,max);
    distright = DistanceTo(collider->box,1,0,0,max);
    distleft = DistanceTo(collider->box,-1,0,PI,max);
}

int Physics::DistanceTo(Rect box,int xsum,int ysum,float angle,int max){
    int distance = 0;

    // Vec2 center( box.GetCenter() );
	// Vec2 points[4];
    // points[0] = (Vec2(box.x + 1, box.y + 1) - center).GetRotated( (associated->angleDeg * PI) /180 ) + center;
    // points[1] = (Vec2(box.x + box.w - 1, box.y + 1) - center).GetRotated( (associated->angleDeg * PI) /180 ) + center;
	// points[2] = (Vec2(box.x + box.w - 1, box.y + box.h - 1) - center).GetRotated( (associated->angleDeg * PI) /180 ) + center;
	// points[3] = (Vec2(box.x + 1, box.y + box.h - 1) - center).GetRotated( (associated->angleDeg * PI) /180 ) + center;

    // Vec2 pointsTo[4];
    // Rect lineboxes[4];
    // for(int i = 0;i < 4;i++){
    //     pointsTo[i] = Vec2(max,0).GetRotated(angle) + points[i];
    //     lineboxes[i] = GetLineBox(points[i],pointsTo[i]);
    // }

    // for(int i = 0;i < 4;i++){
    //     if(IsColliding(lineboxes[i],angle)){
    //         float interval = lineboxes[i].w/2;
    //         while(interval > 0.5){
    //             if(IsColliding(lineboxes[i],angle)){
    //                 lineboxes[i].w -= interval;
    //                 interval /= 2;
    //             }
    //             else if(!IsColliding(lineboxes[i],angle)){
    //                 lineboxes[i].w += interval;
    //                 interval /= 2;
    //             }
    //             Vec2 vectorRot = Vec2(lineboxes[i].w,0).GetRotated(angle) + points[i];
    //             lineboxes[i] = Rect(vectorRot.x - (lineboxes[i].w * (((cos(std::fabs(angle))) + 1)/2) ),vectorRot.y + (lineboxes[i].w/2 * -sin(angle)),lineboxes[i].w,1);
    //         }
    //     }else{
    //         lineboxes[i].w = max + 1;
    //     }
    //     if(std::fabs(lineboxes[i].w) < 1){
    //         lineboxes[i].w = 0;
    //     }else{
    //         lineboxes[i].w = floor(lineboxes[i].w - 1);
    //     }
    // }

    // float dists[] = { lineboxes[0].w,lineboxes[1].w,lineboxes[2].w,lineboxes[3].w};
    // int size = sizeof(dists)/sizeof(dists[0]);
    // std::sort(dists,dists+size);
    // float finaldist = dists[0];
    // std::cout << finaldist << std::endl;
    
    // for(int i = 0;i < 4;i++){
        // WindowEffects::AddBoxToDraw(lineboxes[i],points[i].GetAngle(pointsTo[i]));
    // }


    while(!IsColliding(box,(associated->angleDeg * PI) /180) && (distance < max)){
        box.y += ysum;
        box.x += xsum;
        distance ++;
    }
    while(IsColliding(box,(associated->angleDeg * PI) /180) && (distance > -max)){
        box.y += -ysum;
        box.x += -xsum;
        distance --;
    }
    return distance;
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

Rect Physics::GetLineBox(Vec2 vector,Vec2 vectorTo){
    float distance = vector.GetDistance(vectorTo.x,vectorTo.y);
    float angle = vector.GetAngle(vectorTo.x,vectorTo.y);
    Vec2 vectorRot = Vec2(distance,0).GetRotated(angle) + vector;
    return Rect(vectorRot.x - (distance * (((cos(std::fabs(angle))) + 1)/2) ),vectorRot.y + (distance/2 * -sin(angle)),distance,1);
}

bool Physics::IsOutofBounds(Rect box,float angle){
    Vec2 center( box.GetCenter() );
    Rect map = Rect(0,0,Camera::limit.x,Camera::limit.y);
	Vec2 points[4];

    points[0] = (Vec2(box.x, box.y) - center).GetRotated( angle ) + center;
		
    points[1] = (Vec2(box.x + box.w, box.y) - center).GetRotated( angle ) + center;
		
	points[2] = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated( angle ) + center;
		
	points[3] = (Vec2(box.x, box.y + box.h) - center).GetRotated( angle ) + center;

    if(!map.Contains(points[0]) || !map.Contains(points[1]) || !map.Contains(points[2]) || !map.Contains(points[3])){
        return true;
    }
    return false;
}
bool Physics::IsColliding(Rect box,float angle,bool nooutofbounds,bool markcollision){
    #ifndef DEBUG
        markcollision = false;
    #endif
    if(IsOutofBounds(box,angle) && !StageState::ChangingMap() && !nooutofbounds){
        return true;
    }
    for(int i = 0;i < TileMap::tiles.size();i ++){
        TileCollider *tilecollider = dynamic_cast<TileCollider*>(TileMap::tiles[i].lock().get());
        if(Collision::IsColliding(box,tilecollider->box,angle,0,markcollision)){
            return true;
        }
    }
    return false;
}

bool Physics::IsUp(int sum){
    return IsColliding(collider->box.Added(0,-sum,0,-collider->box.h),(associated->angleDeg * PI) /180);
}

bool Physics::IsGrounded(int sum){
    return IsColliding(collider->box.Added(0,sum + collider->box.h,0,-collider->box.h),(associated->angleDeg * PI) /180);
}

bool Physics::IsLeft(int sum){
    return IsColliding(collider->box.Added(-sum,0,-collider->box.w,0),(associated->angleDeg * PI) /180);
}

bool Physics::IsRight(int sum){
    return IsColliding(collider->box.Added(sum + collider->box.w,0,-collider->box.w,0),(associated->angleDeg * PI) /180);
}


Vec2 Physics::GetCollisionPoint(Rect owner){
    Vec2 hitboxcenter = owner.GetCenter();
    Vec2 center = collider->box.GetCenter();
    float angle = (associated->angleDeg * PI) /180;
    Vec2 point;
    if(hitboxcenter.x >= center.x){
        point = (Vec2(collider->box.x, collider->box.y + collider->box.h/2) - center).GetRotated( angle ) + center;
    }else{
        point = (Vec2(collider->box.x + collider->box.w,collider-> box.y + collider->box.h/2) - center).GetRotated( angle ) + center;
    }

    angle = hitboxcenter.GetAngle(point.x,point.y);
    Rect box = GetLineBox(hitboxcenter,point);
    if(IsColliding(box,angle)){
        float interval = box.w/2;
        while(interval > 1){
            if(IsColliding(box,angle)){
                box.w -= interval;
                interval /= 2;
            }
            else if(!IsColliding(box,angle)){
                box.w += interval;
                interval /= 2;
            }
            Vec2 vectorRot = Vec2(box.w,0).GetRotated(angle) + hitboxcenter;
            box = Rect(vectorRot.x - (box.w * (((cos(std::fabs(angle))) + 1)/2) ),vectorRot.y + (box.w/2 * -sin(angle)),box.w,1);
        }
        return Vec2(box.w,0).GetRotated(angle) + hitboxcenter;
    }
    return Vec2(0,0);
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

float Physics::PerformXMovement(float dt){
    if(speed->x != 0){
        float modX = 0;
        if((speed->x * dt) < 0){
            modX = speed->x * dt;
        }
        if(IsColliding(collider->box.Added(modX,0,std::fabs(speed->x * dt),0),(associated->angleDeg * PI) /180)){
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
}

float Physics::PerformYMovement(float dt){
    if(speed->y != 0){
        float modY = 0;
        if((speed->y * dt) < 0){
            modY = speed->y * dt;
        }
        if(IsColliding(collider->box.Added(0,modY,0,std::fabs(speed->y * dt)),(associated->angleDeg * PI) /180)){
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
}

void Physics::PerformGravity(float gravspeed,float dt){
    if(!IsGrounded()){
        // UpdateDists();
        // if(abs(distground) < 3){
        //     associated->box.y += distground;
        // }
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