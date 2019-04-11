#include "../include/Physics.h"
#include "../include/Collider.h"
#include "../include/Component.h"


Physics::Physics(GameObject* associated) : associated(associated){
    distground = 0;
    distceiling = 0;
    distright = 0;
    distleft = 0;
}

Physics::~Physics(){
    associated = nullptr;
}

void Physics::Update(Rect collider){
    Vec2 Sprite[] = {   Vec2(collider.x,collider.y),
                        Vec2(collider.x + collider.w,collider.y),
                        Vec2(collider.x,collider.y + collider.h),
                        Vec2(collider.x + collider.w,collider.y + collider.h)
    };
    distground = DistanceTo(Sprite[2],Sprite[3],0,1);
    distceiling = DistanceTo(Sprite[0],Sprite[1],0,-1);
    distright = DistanceTo(Sprite[1],Sprite[3],1,0);
    distleft = DistanceTo(Sprite[0],Sprite[2],-1,0);
}

void Physics::CorrectDistance(){
    std::map<int,int> dists;
    dists.insert({0,distground});
    dists.insert({1,distceiling});
    dists.insert({2,distright});
    dists.insert({3,distleft});
    std::deque<int> disttofix;
    for(int i = 0;i < 4;i++){
        if(dists[i] < 0){
            if(disttofix.empty()){
                disttofix.push_front(i);
            }else{
                bool inserted = false;
                for(int j = 0;j < disttofix.size();j++){
                    if(dists[i] > dists[disttofix[j]]){
                        disttofix.push_front(i);
                        j = disttofix.size();
                        inserted = true;
                    }
                }
                if(!inserted){
                    disttofix.push_back(i);
                }
            }
        }
    }
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


int Physics::DistanceTo(Vec2 vector1,Vec2 vector2,int xsum,int ysum){
    int distance = 0;
    while(CanMove(vector1,vector2) && (distance < 150)){
        vector1.y += ysum;
        vector2.y += ysum;
        vector1.x += xsum;
        vector2.x += xsum;
        distance ++;
    }
    while(!CanMove(vector1,vector2) && (distance > -150)){
        vector1.y += -ysum;
        vector2.y += -ysum;
        vector1.x += -xsum;
        vector2.x += -xsum;
        distance --;
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