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
    speed.x = 100;
    speed.y = 100;
    hp = 150;
    player = this;
    Sprite *minion =  new Sprite(associated,"assets/img/beltest.png");
    minion->SetScaleX(0.4,0.4);
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(minion);
}

Player::~Player(){
    player = nullptr;

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

    if(input->IsKeyDown(SDLK_w) == true){
    }
    if(input->IsKeyDown(SDLK_s) == true){
    }
    if(input->IsKeyDown(SDLK_d) == true){
        if(CanMove(Sprite[1].Added(speed.x * dt,0),Sprite[3].Added(speed.x * dt,0))){
            associated.box.x += speed.x * dt;
        }
    }
    if(input->IsKeyDown(SDLK_a) == true){
        if(CanMove(Sprite[0].Added(-speed.x * dt,0),Sprite[2].Added(-speed.x * dt,0))){
            associated.box.x -= speed.x * dt;
        }
    }
    if(input->KeyPress(SDLK_SPACE) == true){
        if(distground <= 0){
            speed.y = -300;
        }
    }

    if((distground - (speed.y * dt)) < 0){
        associated.box.y += distground;
    }else{
        associated.box.y += speed.y * dt;
    } 

    if(distground > 0){
        speed.y += 300*dt;
    }
    else if((distground == 0) && (speed.y > 0)){
        speed.y = 0;
    }
    else if((distground < 0) && (speed.y >= 0)){
        speed.y = 0;
        associated.box.y += distground;
    }

    if(hp <= 0){
        associated.RequestDelete();
    }
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