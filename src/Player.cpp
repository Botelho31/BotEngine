#include "../include/Player.h"
#include "../include/Collider.h"
#include "../include/Camera.h"
#include "../include/Sound.h"
#include "../include/Sprite.h"
#include "../include/TileMap.h"
#include "../include/StageState.h"
#include "../include/Vec2.h"
#include "../include/HitBox.h"
#include "../include/Minion.h"
#include "../include/GameData.h"

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
    jumpanimation =  new Timer();
    falling = false;

    swordarc = -1;
    asword = (PI*2);
    swordattack = new Timer();

    hp = 150;
    invincibilitytimer = new Timer();
    player = this;

    idletimer = new Timer();
    idle = false;

    input =  &(InputManager::GetInstance());
    this->physics = new Physics(&associated);

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
    delete invincibilitytimer;
    delete swordattack;
    delete jumpanimation;
    delete physics;
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
    physics->Update(collider->box);
    #ifdef DEBUG
        if(input->IsKeyDown(SDLK_MINUS)){
            std::cout << "dground: "<< physics->distground << std::endl;
            std::cout << "dceiling: "<< physics->distceiling << std::endl;
            std::cout << "dright: "<< physics->distright << std::endl;
            std::cout << "dleft: "<< physics->distleft << std::endl;
        }
    #endif
    physics->CorrectDistance();

    //HANDLING ATTACK
    if(input->MousePress(SDL_BUTTON_LEFT) == true){    //TESTING SWORD ON W
        if(!swordattack->Started()){
            swordarc = -1;
            Vec2 vector = Vec2(120,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
            Rect hitbox = Rect(vector.x - 20,vector.y - 50,40,100);
            GameObject *swordObj = new GameObject();
            std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
            HitBox *swordhitbox = new HitBox(*swordObj,hitbox,owner,75,0,0.3,0.3,true,false,true,{400,400});
            swordhitbox->SetFunction(SwordHitbox);
            swordObj->AddComponent(swordhitbox);
            Game::GetInstance().GetCurrentState().AddObject(swordObj);
            if(physics->distground > 0){
                speed.y = -200;
            }
            idle = false;
            idletimer->Restart();
            swordattack->Delay(dt);
        }
    }
    if(swordattack->Started()){
        swordattack->Update(dt);
        if(swordattack->Get() >= 0.3){
            swordattack->Restart();
        }
    }
    //END HANDLING ATTACK


    if(input->IsKeyDown(SDLK_s) == true){   //CROUCH?
    }

    XMovement(dt); //X MOVEMENT
    YMovement(dt); //Y MOVEMENT
    IdleHandle(dt);//IDLE HANDLING

    if(invincibilitytimer->Started()){
        invincibilitytimer->Update(dt);
        if(invincibilitytimer->Get() >= 2){
            invincibilitytimer->Restart();
        }
    }
    if(hp <= 0){
        // Camera::UnFollow();
        // associated.RequestDelete();
        GameData::playerAlive = false;
    }
}

void Player::XMovement(float dt){
    //Handles input and acceleration
    if(input->IsKeyDown(SDLK_d) == true){
        if((idle == true) && (physics->distground == 0)){
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
        if((idle == true) && (physics->distground == 0)){
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

    if(((input->IsKeyDown(SDLK_a) == false) && (input->IsKeyDown(SDLK_d) == false)) && (physics->distground == 0)){
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

    //Perfoms Movement if Allowed
    if((physics->distright - (speed.x * dt)) < 0){
        associated.box.x += physics->distright;
        speed.x = 0;
    }else if((physics->distleft + (speed.x * dt)) < 0){
        associated.box.x -= physics->distleft;
        speed.x = 0;
    }else if((physics->distright < 0) && (speed.x > 0)){
        speed.x = 0;
    }else if((physics->distleft < 0) && (speed.x < 0)){
        speed.x = 0;
    }else{
        associated.box.x += speed.x * dt;
    }
    
}
void Player::YMovement(float dt){

    //Handles when it hits the ground
    if((physics->distground <= 0) && (speed.y > 0)){
        speed.y = 0;
        falling = false;
        SetSprite("assets/img/belhitthegroundtest3.png",4,0.08,false);
        SetCollider(0.276,1);
        hittheground->Delay(dt);
    }
    if(hittheground->Started()){
        speed.y = 0;
        hittheground->Update(dt);
        if(hittheground->Get() >= 0.24){
            SetSprite("assets/img/beltest2.png");
            SetCollider(0.6,1);
            hittheground->Restart();
        }
    }

    //Handles Jump input and acceleration
    if((input->KeyPress(SDLK_SPACE) == true) && (hittheground->Get() == 0)){
        if(physics->distground <= 0){
            if(idle == true){
                idletimer->Restart();
                idle = false;
            }
            SetSprite("assets/img/beljumptest4.png",15,0.04,false,{0,-10});
            SetCollider(0.261,0.8);
            jumpanimation->Delay(dt);
            jumpsquat->Delay(dt);
        }else if(physics->distright == 0){
            speed.y = ajump;
            speed.x = -awalljump;
        }else if(physics->distleft == 0){
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
    if(jumpanimation->Started()){
        jumpanimation->Update(dt);
        if(jumpanimation->Get() >= 0.6){
            jumpanimation->Restart();
        }
    }

    //Handles when it is falling
    if((physics->distground > 0) && (speed.y > 0) && (falling == false) && (!jumpanimation->Started())){
        SetSprite("assets/img/belfreefallingtest.png",4,0.04);
        SetCollider(0.261,0.8);
        falling = true;
    }
    
    //Performs movement if it is allowed
    if(((physics->distground - (speed.y * dt)) < 0) && (speed.y > 0)){
        associated.box.y += physics->distground;
    }
    else if((physics->distceiling + (speed.y * dt) < 0) && (speed.y < 0)){
        associated.box.y -= physics->distceiling;
        speed.y = 0;
    } 
    else{
        associated.box.y += speed.y * dt;
    }

    //GRAVITY
    if((physics->distground > 0) && (jumpsquat->Get() == 0)){
        if(idle == true){
            idletimer->Restart();
            idle = false;
        }
        speed.y += gravspeed*dt;
    }
}

void Player::IdleHandle(float dt){
    if((idle == false) && (((speed.x == 0) && (speed.y == 0)) && ((input->IsKeyDown(SDLK_a) == false) && (input->IsKeyDown(SDLK_d) == false)))){
        idletimer->Update(dt);
        if((idletimer->Get() > 2) && (idle == false)){
            SetSprite("assets/img/belidletest2.png",8,0.08);
            idle = true;
        }
    }else{
        idletimer->Restart();
    }
}

void Player::DamagePlayer(int damage){
    hp -= damage;
}

void Player::HealPlayer(int heal){
    hp += heal;
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
    if(invincibilitytimer->Started()){

    }else{
        Component *component1 = other.GetComponent("HitBox");
        Component *component2 = other.GetComponent("Minion");
        if(component1){
            HitBox *hitbox = dynamic_cast<HitBox*>(component1);
            if(hitbox->GetOwner() && hitbox->HitPlayer()){
                physics->KnockBack(hitbox->GetOwner()->box,&speed,hitbox->GetKnockBack());
                DamagePlayer(hitbox->GetDamage());
                invincibilitytimer->Delay(0);
            }
        }
        else if(component2){
            Component *collidercomponent = other.GetComponent("Collider");
            Collider *collider = dynamic_cast<Collider*>(collidercomponent);
            physics->KnockBack(collider->box,&speed,Vec2(400,400));
            invincibilitytimer->Delay(0);
        }
    }
}

Vec2 Player::GetPosition(){
    return Vec2(associated.box.x + associated.box.w/2,associated.box.y + associated.box.h/2);
}

Vec2 Player::GetSpeed(){
    return Vec2(speed.x,speed.y);
}

int Player::GetLife(){
    return hp;
}