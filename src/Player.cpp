#include "../include/Player.h"
#include "../include/Collider.h"
#include "../include/Camera.h"
#include "../include/Sound.h"
#include "../include/Sprite.h"
#include "../include/TileMap.h"
#include "../include/StageState.h"
#include "../include/Vec2.h"
#include "../include/HitBox.h"

Player *Player::player;

Player::Player(GameObject& associated) : Component(associated){
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    ajump = -1150;
    awalljump = 400;
    gravspeed = 2000;
    jumpsquat = new Timer();
    hittheground = new Timer();

    swordarc = -1;
    asword = (PI*2);

    hp = 150;
    player = this;
    idletimer = new Timer();
    idle = false;
    input =  &(InputManager::GetInstance());
    Sprite *player =  new Sprite(associated,"assets/img/beltest2.png");
    this->playersprite = player;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(player);
    SetCollider(0.6,1);
}

Player::~Player(){
    player = nullptr;
    playersprite = nullptr;
    input = nullptr;
    delete idletimer;
    delete jumpsquat;
    delete hittheground;

}

void Player::Start(){
}

void Player::SwordHitbox(GameObject& hitbox,GameObject& owner,float dt){
    Component *component1 = owner.GetComponent("Player");
    Player *player = dynamic_cast<Player*>(component1);
    Component *component2 = owner.GetComponent("Collider");
    Collider *collider = dynamic_cast<Collider*>(component2);


    hitbox.angleDeg += 70 * dt;
    player->swordarc += player->asword * dt;
    Vec2 vector = Vec2(120,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
    hitbox.box.Transform(vector.x - hitbox.box.w/2,vector.y - hitbox.box.h/2);
    
}

void Player::Update(float dt){
    Component *component = associated.GetComponent("Collider");
    Collider *collider = dynamic_cast<Collider*>(component);
    Vec2 Sprite[] = {   Vec2(collider->box.x,collider->box.y),
                        Vec2(collider->box.x + collider->box.w,collider->box.y),
                        Vec2(collider->box.x,collider->box.y + collider->box.h),
                        Vec2(collider->box.x + collider->box.w,collider->box.y + collider->box.h)
    };
    distground = DistanceTo(Sprite[2],Sprite[3],0,1);
    distceiling = DistanceTo(Sprite[0],Sprite[1],0,-1);
    distright = DistanceTo(Sprite[1],Sprite[3],1,0);
    distleft = DistanceTo(Sprite[0],Sprite[2],-1,0);
    // std::cout << "dground: "<< distground << std::endl;
    // std::cout << "dceiling: "<< distceiling << std::endl;
    // std::cout << "dright: "<< distright << std::endl;
    // std::cout << "dleft: "<< distleft << std::endl;

    CorrectDistance();

    if(input->MousePress(SDL_BUTTON_LEFT) == true){    //TESTING SWORD ON W
        swordarc = -1;
        Vec2 vector = Vec2(120,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
        Rect hitbox = Rect(vector.x - 20,vector.y - 50,40,100);
        GameObject *swordObj = new GameObject();
        std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
        HitBox *swordhitbox = new HitBox(*swordObj,owner,hitbox,0,0,true,true,0.3,75);
        swordhitbox->SetFunction(SwordHitbox);
        swordObj->AddComponent(swordhitbox);
        Game::GetInstance().GetCurrentState().AddObject(swordObj);
    }


    if(input->IsKeyDown(SDLK_s) == true){   //CROUCH?
    }

    //X MOVEMENT
    XMovement(dt);

    //Y MOVEMENT
    YMovement(dt);

    //IDLE HANDLING
    if((idle == false) && (((speed.x == 0) && (speed.y == 0)) && ((input->IsKeyDown(SDLK_a) == false) && (input->IsKeyDown(SDLK_d) == false)))){
        idletimer->Update(dt);
        if((idletimer->Get() > 2) && (idle == false)){
            SetSprite("assets/img/belidletest2.png",8,0.08);
            idle = true;
        }
    }else{
        idletimer->Restart();
    }
    //END IDLE HANDLING

    if(hp <= 0){
        associated.RequestDelete();
    }
}

void Player::XMovement(float dt){
    if(input->IsKeyDown(SDLK_d) == true){
        if((idle == true) && (distground == 0)){
            SetSprite("assets/img/beltest2.png");
            SetCollider(0.6,1);
            idle = false;
        }
        if(playersprite->IsFlipped()){
            playersprite->Flip();
        }
        if(speed.x >= 0){
            if((speed.x + aspeed*dt) > maxspeed){
                speed.x = maxspeed;
            }else{
                speed.x += aspeed * dt;
            }
        }else{
           speed.x += aspeed * dt + despeed * dt; 
        }
    }
    if(input->IsKeyDown(SDLK_a) == true){
        if((idle == true) && (distground == 0)){
            SetSprite("assets/img/beltest2.png");
            SetCollider(0.6,1);
            idle = false;
        }
        if(!playersprite->IsFlipped()){
            playersprite->Flip();
        }
        if(speed.x <= 0){
            if(abs(speed.x - aspeed*dt) > maxspeed){
                speed.x = -maxspeed;
            }else{
                speed.x -= aspeed * dt;
            }
        }else{
            speed.x -= aspeed *dt + despeed *dt;
        }
    }

    if(((input->IsKeyDown(SDLK_a) == false) && (input->IsKeyDown(SDLK_d) == false)) && (distground == 0)){
        if(speed.x > 0){
            if((speed.x - despeed * dt) < 0){
                speed.x = 0;
            }else{
                speed.x -= despeed * dt;
            }
        }else{
            if((speed.x + despeed * dt) > 0){
                speed.x = 0;
            }else{
                speed.x += despeed * dt;
            }
        }
    }
    if((distright - (speed.x * dt)) < 0){
        associated.box.x += distright;
        speed.x = 0;
    }else if((distleft + (speed.x * dt)) < 0){
        associated.box.x -= distleft;
        speed.x = 0;
    }else if((distright < 0) && (speed.x > 0)){
        speed.x = 0;
    }else if((distleft < 0) && (speed.x < 0)){
        speed.x = 0;
    }else{
        associated.box.x += speed.x * dt;
    }
    
}
void Player::YMovement(float dt){
    //Y MOVEMENT
    if((input->KeyPress(SDLK_SPACE) == true) && (hittheground->Get() == 0)){
        if(distground <= 0){
            if(idle == true){
                idletimer->Restart();
                idle = false;
            }
            SetSprite("assets/img/beljumptest4.png",15,0.04,false,{0,-10});
            SetCollider(0.261,0.8);
            jumpsquat->Delay(dt);
        }else if(distright == 0){
            speed.y = ajump;
            speed.x = -awalljump;
        }else if(distleft == 0){
            speed.y = ajump;
            speed.x = awalljump;
        }
    }
    if(jumpsquat->Started()){
        speed.y = 0;
        jumpsquat->Update(dt);
        if(jumpsquat->Get() >= 0.12){
            speed.y = ajump;
            jumpsquat->Restart();
        }
    }
    if((distground <= 0) && (speed.y > 0)){
        speed.y = 0;
        falling = false;
        SetSprite("assets/img/belhitthegroundtest2.png",16,0.02,false);
        SetCollider(0.2264150,1);
        hittheground->Delay(dt);
    }
    if(hittheground->Started()){
        speed.y = 0;
        hittheground->Update(dt);
        if(hittheground->Get() >= 0.36){
            SetSprite("assets/img/beltest2.png");
            SetCollider(0.6,1);
            hittheground->Restart();
        }
    }
    if((distground > 0) && (speed.y > 0) && (falling == false) && (playersprite->GetHeight() < 220)){
        SetSprite("assets/img/beljumptest4.png",15,0.04,false);
        playersprite->SetFrame(14);
        playersprite->SetFrameTime(0);
        SetCollider(0.261,0.8);
        falling = true;
    }
    if(((distground - (speed.y * dt)) < 0) && (speed.y > 0)){
        associated.box.y += distground;
    }
    else if((distceiling + (speed.y * dt) < 0) && (speed.y < 0)){
        associated.box.y -= distceiling;
        speed.y = 0;
    } 
    else{
        associated.box.y += speed.y * dt;
    }
    //END Y MOVEMENT

    //GRAVITY
    if((distground > 0) && (jumpsquat->Get() == 0)){
        if(idle == true){
            idletimer->Restart();
            idle = false;
        }
        speed.y += gravspeed*dt;
    }
    //END GRAVITY
}

void Player::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    playersprite->SetFrameCount(framecount);
    playersprite->SetFrameTime(frametime);
    playersprite->SetRepeat(repeat);
    playersprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
}

void Player::SetCollider(float scaleX,float scaleY,float offsetX,float offsetY){
    Component *component = associated.GetComponent("Collider");
    Collider *collider = dynamic_cast<Collider*>(component);
    collider->SetScale(Vec2(scaleX,scaleY));
    collider->SetOffSet(Vec2(offsetX,offsetY));
}

void Player::CorrectDistance(){
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


int Player::DistanceTo(Vec2 vector1,Vec2 vector2,int xsum,int ysum){
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

bool Player::CanMove(Vec2 vector1,Vec2 vector2){
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

void Player::MovePlayer(float x,float y){
    associated.box.x = x - associated.box.w/2;
    associated.box.y = y - associated.box.h/2;
}

void Player::Render(){

}

bool Player::Is(std::string type){
    if(type == "Player"){
        return true;
    }else{
        return false;
    }
}

void Player::NotifyCollision(GameObject& other){
}

Vec2 Player::GetPosition(){
    return Vec2(associated.box.x + associated.box.w/2,associated.box.y + associated.box.h/2);
}

Vec2 Player::GetSpeed(){
    return Vec2(speed.x,speed.y);
}