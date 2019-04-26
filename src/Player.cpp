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
    aspeed = 1000;
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
    currentAttack = 0;
    attacktiming = 0;
    endofattack = 0;
    delayedboost = 0;
    swordattack = new Timer();
    delayedboosttimer = new Timer();

    hp = 150;
    invencible = false;
    invincibilitytimer = new Timer();
    player = this;

    idletimer = new Timer();
    idle = false;

    runningstarttimer = new Timer();
    runningstoptimer = new Timer();
    running = false;

    input =  &(InputManager::GetInstance());
    this->physics = new Physics(&associated,&speed);

    Sprite *player =  new Sprite(associated,"assets/img/belidleswordtest.png",32,0.08);
    this->playersprite = player;
    associated.AddComponent(player);
    physics->SetCollider(0.276,1);
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
    delete delayedboosttimer;
    delete jumpanimation;
    delete runningstarttimer;
    delete runningstoptimer;
    delete physics;
}

void Player::Start(){
}

void Player::Update(float dt){
    Collider *collider = physics->GetCollider();
    physics->Update(collider->box);
    #ifdef DEBUG
        if(input->IsKeyDown(SDLK_MINUS)){
            std::cout << "dground: "<< physics->distground << std::endl;
            std::cout << "dceiling: "<< physics->distceiling << std::endl;
            std::cout << "dright: "<< physics->distright << std::endl;
            std::cout << "dleft: "<< physics->distleft << std::endl;
            std::cout << "speedX: " << speed.x << std::endl;
            std::cout << "speedY: " << speed.y << std::endl;
        }
    #endif
    physics->CorrectDistance();


    AttackHandle(dt);//HANDLING ATTACK
    IdleHandle(dt);//IDLE HANDLING
    XMovement(dt); //X MOVEMENT
    YMovement(dt); //Y MOVEMENT

    if(invincibilitytimer->Started()){
        invincibilitytimer->Update(dt);
        if((invincibilitytimer->Get() >= 2) && (invencible == false)){
            invincibilitytimer->Restart();
        }
    }

    if(hp <= 0){
        GameData::playerAlive = false;
    }
}

void Player::SwordHitbox(GameObject& hitbox,GameObject& owner,float dt){
    Component *component1 = owner.GetComponent("Player");
    Player *player = dynamic_cast<Player*>(component1);
    Collider *collider = player->physics->GetCollider();

    // hitbox.angleDeg += 70 * dt;
    player->swordarc += player->asword * dt;
    Vec2 vector = Vec2(120,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
    hitbox.box.Transform(vector.x - hitbox.box.w/2,vector.y - hitbox.box.h/2);
    
}

void Player::InstanceHitbox(float asword,float swordarc){
    this->asword = asword;
    this->swordarc = swordarc;
    Collider *collider = physics->GetCollider();
    Vec2 vector = Vec2(130,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
    Rect hitbox = Rect(vector.x - 45,vector.y - 50,90,100);
    GameObject *swordObj = new GameObject();
    std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    HitBox *swordhitbox = new HitBox(*swordObj,hitbox,owner,90,20,0.2,0.4,true,false,true,{700,300});
    swordhitbox->SetFunction(SwordHitbox);
    swordObj->AddComponent(swordhitbox);
    Game::GetInstance().GetCurrentState().AddObject(swordObj);
}

void Player::AttackHandle(float dt){
    //HANDLING ATTACK
    if(input->KeyPress(SDLK_e) == true){    //TESTING SWORD ON E
        if(currentAttack == 1){
            if(nextattack.size() <  2){
                nextattack.push(1);
            }
        }
        if(currentAttack == 0){
            currentAttack = 1;
        }
    }
    if((currentAttack != 0) && (!swordattack->Started())){
        if(currentAttack == 1){
            SetSprite("assets/img/belattacktest.png",22,0.04,false);
            physics->SetCollider(0.15054545,1);
            attacktiming = 0.4;
            endofattack = 1;
            if(playersprite->IsFlipped()){
                InstanceHitbox(-(PI/0.4),-PI + 1);
            }else{
                InstanceHitbox((PI/0.4),-1);
            }
            swordattack->Delay(dt);
            delayedboost = 0.12;
            delayedboosttimer->Delay(dt);
        }
        if(!physics->IsGrounded() && (physics->distground > 100)){
            speed.y = -400;
        }
    }
    if(delayedboosttimer->Started()){
        delayedboosttimer->Update(dt);
        if(!physics->IsGrounded()){
            delayedboosttimer->Restart();
        }
        if(delayedboosttimer->Get() >= delayedboost){
            if(playersprite->IsFlipped()){
                speed.x = -500;
            }else{
                speed.x = 500;
            }
            delayedboosttimer->Restart();
        }
    }
    if(swordattack->Started()){
        swordattack->Update(dt);
        running = false;
        if(physics->IsGrounded()){
            physics->PerformXDeceleration(1500,dt);
        }else{
             physics->PerformXDeceleration(750,dt);
        }
        if((swordattack->Get() >= attacktiming) && ((!nextattack.empty()))){
            currentAttack = nextattack.front();
            nextattack.pop();
            speed.x = 0;
            swordattack->Restart();
        }
        if(swordattack->Get() >= endofattack){
            speed.x = 0;
            currentAttack = 0;
            if(physics->IsGrounded()){
                SetSprite("assets/img/belidleswordtest.png",32,0.08);
                physics->SetCollider(0.276,1);
            }else{
                SetSprite("assets/img/belfreefallingtest3.png",4,0.04);
                physics->SetCollider(0.261,0.8);
            }
            swordattack->Restart();
        }
    }
    if((swordattack->Get() >= attacktiming) && (input->IsKeyDown(SDLK_d) || input->IsKeyDown(SDLK_a) || input->IsKeyDown(SDLK_SPACE))){
        currentAttack = 0;
        for(int i = 0;i < nextattack.size();i++){
            nextattack.pop();
        }
        swordattack->Restart();
    }
}

void Player::XMovement(float dt){
    //Handles input and acceleration
    if(!physics->IsGrounded()){
        running = false;
    }

    if(input->IsKeyDown(SDLK_d)){
        if((running == false) && (physics->IsGrounded()) && (!hittheground->Started())  && (!swordattack->Started())){
            SetSprite("assets/img/belstartwalktest.png",4,0.04,false);
            physics->SetCollider(0.2484,1);
            runningstarttimer->Restart();
            runningstarttimer->Delay(dt);
            running = true;
        }
        if(playersprite->IsFlipped() && (!swordattack->Started())){
            playersprite->Flip();
            speed.x = 0;
        }
        physics->PerformXAcceleration(true,aspeed,maxspeed,despeed,dt);
    }
    if(input->IsKeyDown(SDLK_a)){
        if((running == false) && (physics->IsGrounded()) && (!hittheground->Started())  && (!swordattack->Started())){
            SetSprite("assets/img/belstartwalktest.png",4,0.04,false);
            physics->SetCollider(0.2484,1);
            runningstarttimer->Restart();
            runningstarttimer->Delay(dt);
            running = true;
        }
        if(!playersprite->IsFlipped() && (!swordattack->Started())){
            playersprite->Flip();
            speed.x = 0;
        }
        physics->PerformXAcceleration(false,aspeed,maxspeed,despeed,dt);
    }

    if(runningstarttimer->Started()){
        runningstarttimer->Update(dt);
        if((running == false) || (!physics->IsGrounded())  || (swordattack->Started())){
            runningstarttimer->Restart();
        }
        if(runningstarttimer->Get() >= 0.16){
            SetSprite("assets/img/belwalktest4.png",14,0.04);
            physics->SetCollider(0.184,1);
            runningstarttimer->Restart();
        }
    }

    if(((input->IsKeyDown(SDLK_a) == false) && (input->IsKeyDown(SDLK_d) == false)) && (physics->IsGrounded())){
        physics->PerformXDeceleration(despeed,dt);
        if(running == true){
            SetSprite("assets/img/belstoptest2.png",2,0.04,false);
            physics->SetCollider(0.184,1);
            runningstoptimer->Delay(dt);
            running = false;
        }
    }

    if(runningstoptimer->Started()){
        runningstoptimer->Update(dt);
        if((!physics->IsGrounded())  || (swordattack->Started())){
            runningstoptimer->Restart();
        }
        if(runningstoptimer->Get() >= 0.08){
            speed.x = 0;
            SetSprite("assets/img/belidleswordtest.png",32,0.08);
            physics->SetCollider(0.276,1);
            runningstoptimer->Restart();
        }
    }

    physics->PerformXMovement(dt);//Perfoms Movement if Allowed
}

void Player::YMovement(float dt){

    //Handles when it hits the ground
    if((physics->IsGrounded()) && (speed.y > 0)){
        speed.y = 0;
        falling = false;
        if(!swordattack->Started()){
            SetSprite("assets/img/belhitthegroundtest4.png",4,0.04,false);
            physics->SetCollider(0.276,1);
            hittheground->Delay(dt);
        }
    }
    if(hittheground->Started()){
        speed.y = 0;
        hittheground->Update(dt);
        if(hittheground->Get() >= 0.12){
            if(!swordattack->Started()){
                SetSprite("assets/img/belidleswordtest.png",32,0.08);
                physics->SetCollider(0.276,1);
            }
            hittheground->Restart();
        }
    }

    //Handles Jump input and acceleration
    if((input->KeyPress(SDLK_SPACE)) && (!hittheground->Started())){
        if(!swordattack->Started()){
            if(physics->IsGrounded()){
                running = false;
                SetSprite("assets/img/beljumptest4.png",15,0.04,false,{0,-10});
                physics->SetCollider(0.261,0.8);
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
    if((!physics->IsGrounded()) && (speed.y > 0) && (falling == false) && (!jumpanimation->Started()) && (!swordattack->Started())){
        SetSprite("assets/img/belfreefallingtest3.png",4,0.04);
        physics->SetCollider(0.261,0.8);
        falling = true;
    }
    
    physics->PerformYMovement(dt); //Performs movement if it is allowed

    //GRAVITY
    if(!jumpsquat->Started()){
        physics->PerformGravity(gravspeed,dt);
    }
}

void Player::IdleHandle(float dt){
    if((speed.x == 0) && (speed.y == 0) && (running == false) && (physics->IsGrounded()) && (!jumpsquat->Started())  && (!swordattack->Started())){
        idletimer->Update(dt);
        if((idletimer->Get() > 2) && (idle == false)){
            // SetSprite("assets/img/belidleswordtest.png",32,0.08);
            // physics->SetCollider(0.276,1);
            idle = true;
        }
    }else{
        idle = false;
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

void Player::SetSpeed(Vec2 speed){
    this->speed.x = speed.x;
    this->speed.y = speed.y;
}

void Player::KeepStill(bool freeze,float time){
    this->playersprite->KeepStill(freeze,time);
    Component::KeepStill(freeze,time);
}

void Player::SetInvincibility(bool Invencible){
    this->invencible = Invencible;
    invincibilitytimer->Delay(0);
}

void Player::MovePlayer(float x,float y,bool keepMomentum){
    if(!keepMomentum){
        speed.y = 0;
        speed.x = 0;
    }
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
    if(!invincibilitytimer->Started()){
        Component *component1 = other.GetComponent("HitBox");
        Component *component2 = other.GetComponent("Minion");
        if(component1){
            HitBox *hitbox = dynamic_cast<HitBox*>(component1);
            if(hitbox->GetOwner()){
                if(hitbox->HitPlayer()){
                    physics->KnockBack(hitbox->GetOwner()->box,hitbox->GetKnockBack());
                    DamagePlayer(hitbox->GetDamage());
                    invincibilitytimer->Delay(0);
                }else{
                    KeepStill(true,0.2);
                }
            }
        }
        else if(component2){
            Component *collidercomponent = other.GetComponent("Collider");
            Collider *collider = dynamic_cast<Collider*>(collidercomponent);
            physics->KnockBack(collider->box,Vec2(400,400));
            DamagePlayer(10);
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