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
    Sprite *minion =  new Sprite(associated,"assets/img/miniontest.png");
    minion->SetScaleX(0.2,0.2);
    Collider *collider = new Collider(associated,{0.5,0.5});
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

    if(input->IsKeyDown(SDLK_w) == true){
    }
    if(input->IsKeyDown(SDLK_s) == true){
    }
    if(input->IsKeyDown(SDLK_d) == true){
        associated.box.x += speed.x * dt;
    }
    if(input->IsKeyDown(SDLK_a) == true){
        associated.box.x -= speed.x * dt;
    }
    Vec2 startofsprite = Vec2(associated.box.x,associated.box.y + associated.box.h);
    Vec2 endofsprite = Vec2(associated.box.x + associated.box.w,associated.box.y + associated.box.h);
    if((tilemap->AtLocation(startofsprite.x,startofsprite.y - 20) == -1) && (tilemap->AtLocation(endofsprite.x,endofsprite.y - 20) == -1)){
        speed.y += 100*dt;
        associated.box.y += speed.y * dt;
    }else{
        speed.y = 100;
    }

    if(hp <= 0){
        associated.RequestDelete();
    }
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