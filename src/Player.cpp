#include "../include/Player.h"
#include "../include/Collider.h"
#include "../include/Camera.h"
#include "../include/Sound.h"
#include "../include/Sprite.h"
#include "../include/TileMap.h"
#include "../include/StageState.h"
#include "../include/Vec2.h"

Player *Player::player;

Player::Player(GameObject& associated) : Component(associated){
    speed.x = 300;
    speed.y = 0;
    hp = 150;
    player = this;
    idletimer = new Timer();
    idle = false;
    Sprite *player =  new Sprite(associated,"assets/img/beltest.png");
    player->SetScaleX(0.5,0.5);
    this->playersprite = player;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(player);
}

Player::~Player(){
    player = nullptr;
    playersprite = nullptr;
    delete idletimer;

}

void Player::Start(){
}

void Player::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    TileMap *tilemap = Game::GetInstance().GetCurrentState().GetTileMap();

    Vec2 Sprite[] = {   Vec2(associated.box.x,associated.box.y),
                        Vec2(associated.box.x + associated.box.w,associated.box.y),
                        Vec2(associated.box.x,associated.box.y + associated.box.h),
                        Vec2(associated.box.x + associated.box.w,associated.box.y + associated.box.h)
    };
    int distground = DistanceToGround(Sprite[2],Sprite[3]);
    int distceiling = DistanceToCeiling(Sprite[0],Sprite[1]);

    if(input->IsKeyDown(SDLK_w) == true){
    }
    if(input->IsKeyDown(SDLK_s) == true){
    }

    //X MOVEMENT
    if(input->IsKeyDown(SDLK_d) == true){
        if(idle == true){
            SetSprite("assets/img/beltest.png",1,1);
            idle = false;
        }
        if(playersprite->IsFlipped()){
            playersprite->Flip();
        }
        if(CanMove(Sprite[1].Added(speed.x * dt,0),Sprite[3].Added(speed.x * dt,0))){
            associated.box.x += speed.x * dt;
        }
    }
    if(input->IsKeyDown(SDLK_a) == true){
        if(idle == true){
            SetSprite("assets/img/beltest.png",1,1);
            idle = false;
        }
        if(!playersprite->IsFlipped()){
            playersprite->Flip();
        }
        if(CanMove(Sprite[0].Added(-speed.x * dt,0),Sprite[2].Added(-speed.x * dt,0))){
            associated.box.x -= speed.x * dt;
        }
    }
    //END X MOVEMENT

    //Y MOVEMENT
    if(input->KeyPress(SDLK_SPACE) == true){
        if(distground <= 0){
            if(idle == true){
                SetSprite("assets/img/beltest.png",1,1);
                idle = false;
            }
            speed.y = -600;
        }
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
    if(distground > 0){
        if(idle == true){
            SetSprite("assets/img/beltest.png",1,1);
            idle = false;
        }
        speed.y += 600*dt;
    }
    else if((distground == 0) && (speed.y > 0)){
        speed.y = 0;
    }
    else if((distground < 0) && (speed.y >= 0)){
        speed.y = 0;
        associated.box.y += distground;
    }
    //END GRAVITY

    //IDLE HANDLING
    if((idle == false) && ((speed.y == 0) && ((input->IsKeyDown(SDLK_a) == false) && (input->IsKeyDown(SDLK_d) == false)))){
        idletimer->Update(dt);
        if((idletimer->Get() > 3) && (idle == false)){
            SetSprite("assets/img/belidletest.png",8,0.08);
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

void Player::SetSprite(std::string file,int framecount,float frametime){
    playersprite->SetFrameCount(framecount);
    playersprite->SetFrameTime(frametime);
    playersprite->Open(file);
}

int Player::DistanceToGround(Vec2 vector1,Vec2 vector2){
    int distance = 0;
    while(CanMove(vector1,vector2)){
        vector1.y ++;
        vector2.y ++;
        distance ++;
    }
    while(!CanMove(vector1,vector2)){
        vector1.y --;
        vector2.y --;
        distance --;
    }
    return distance;
}

int Player::DistanceToCeiling(Vec2 vector1,Vec2 vector2){
    int distance = 0;
    while(CanMove(vector1,vector2)){
        vector1.y --;
        vector2.y --;
        distance ++;
    }
    while(!CanMove(vector1,vector2)){
        vector1.y ++;
        vector2.y ++;
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
        if(tilemap->AtLocation(vector1.x,vector1.y) != -1){
            return false;
        }
        vector1.x += x;
        vector1.y += y;
    }
    return true;
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