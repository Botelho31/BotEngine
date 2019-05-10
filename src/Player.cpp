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
    aspeed = 2000;
    despeed = 2000;

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
    aswordangle = 70;
    swordradius = 95;
    attacktiming = 0;
    endofattack = 0;
    delayedboost = 0;
    swordattack = new Timer();
    delayedboosttimer = new Timer();

    hp = 150;
    invencible = false;
    invincibilitytimer = new Timer();
    damagetimer = new Timer();
    player = this;

    idletimer = new Timer();
    idle = false;

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
    delete damagetimer;
    delete swordattack;
    delete delayedboosttimer;
    delete jumpanimation;
    delete runningstoptimer;
    delete physics;
}

void Player::Start(){
}

void Player::Update(float dt){
    Collider *collider = physics->GetCollider();
    physics->Update(collider->box,150);
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

    if(damagetimer->Started()){
        damagetimer->Update(dt);
        if( (running) || (hittheground->Started())  || (swordattack->Started()) || (jumpanimation->Started())){
            damagetimer->Restart();
        }
        if(damagetimer->Get() > 0.21){
            damagetimer->Restart();
            if(physics->IsGrounded()){
                SetSprite("assets/img/belidleswordtest.png",32,0.08);
                physics->SetCollider(0.276,1);
            }else{
                SetSprite("assets/img/belfreefallingtest3.png",4,0.04);
                physics->SetCollider(0.276,0.8);
            }
        }
    }
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

    hitbox.angleDeg += player->aswordangle * dt;
    player->swordarc += player->asword * dt;
    Vec2 vector = Vec2(player->swordradius,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
    hitbox.box.Transform(vector.x - hitbox.box.w/2,vector.y - hitbox.box.h/2);
    
}

void Player::InstanceHitbox(){
    Collider *collider = physics->GetCollider();
    Vec2 vector = Vec2(swordradius,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
    Rect hitbox = Rect(vector.x - 50,vector.y - 30,100,60);
    GameObject *swordObj = new GameObject();
    std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    HitBox *swordhitbox = new HitBox(*swordObj,hitbox,owner,0,20,attacktiming - delayedboost,(attacktiming - delayedboost)*2,true,false,true,{500,100},this,0.1);
    swordhitbox->SetFunction(SwordHitbox);
    swordObj->AddComponent(swordhitbox);
    Game::GetInstance().GetCurrentState().AddObject(swordObj);
}

void Player::AttackHandle(float dt){
    //HANDLING ATTACK
    if(input->KeyPress(SDLK_e) == true){    //TESTING SWORD ON E
        if(nextattack.empty()){
            nextattack.push(1);
        }
        else if(nextattack.back() == 1){
            if(nextattack.size() <  2){
                nextattack.push(2);
            }
        }
        else if(nextattack.back() == 2){
            if(nextattack.size() <  2){
                nextattack.push(1);
            }
        }
    }
    //PERFORMS THE NEXT ATTACK IN QUEUE
    if((!nextattack.empty()) && (!swordattack->Started())){
        if(nextattack.front() == 1){
            SetSprite("assets/img/belattacktest2.png",22,0.04,false);
            physics->SetCollider(0.15771429,1);
            if(playersprite->IsFlipped()){
                player->asword = -((PI * 0.35)/0.22);
                player->swordarc =  -PI + 0.5;
                player->aswordangle = -70;

            }else{
                player->asword = ((PI * 0.35)/0.22);
                player->swordarc =  -0.5;
                player->aswordangle = 70;
            }
            player->swordradius = 100;
            attacktiming = 0.4;
            endofattack = 1;
            swordattack->Delay(dt);
            delayedboost = 0.18;
            delayedboosttimer->Delay(dt);
        }
        if(nextattack.front() == 2){
            SetSprite("assets/img/belattack2test3.png",22,0.04,false);
            physics->SetCollider(0.15771429,1);
            if(playersprite->IsFlipped()){
                player->asword= ((PI * 0.35)/0.22);
                player->swordarc =  PI - 0.5;
                player->aswordangle = 70;

            }else{
                player->asword = -((PI * 0.35)/0.22);
                player->swordarc =  0.5;
                player->aswordangle = -70;
            }
            swordradius = 90;
            attacktiming = 0.4;
            endofattack = 1;
            swordattack->Delay(dt);
            delayedboost = 0.18;
            delayedboosttimer->Delay(dt);
        }
        if(!physics->IsGrounded() && (physics->distground > 100)){
            speed.y = -350;
        }
    }
    //HANDLES START OF HITBOX AND BOOST FROM ATTACK
    if(delayedboosttimer->Started()){
        delayedboosttimer->Update(dt);
        if(delayedboosttimer->Get() >= delayedboost){
            InstanceHitbox();
            if(physics->IsGrounded()){
                if(playersprite->IsFlipped()){
                    speed.x = -500;
                }else{
                    speed.x = 500;
                }
            }
            delayedboosttimer->Restart();
        }
    }
    //HANDLES THE TIMING OF THE ATTACKS
    if(swordattack->Started()){
        swordattack->Update(dt);
        physics->PerformXDeceleration(2000,dt);
        if((swordattack->Get() >= attacktiming) && (nextattack.size() > 1)){
            nextattack.pop();
            speed.x = 0;
            swordattack->Restart();
        }
        if(swordattack->Get() >= endofattack){
            speed.x = 0;
            nextattack.pop();
            if(physics->IsGrounded()){
                SetSprite("assets/img/belidleswordtest.png",32,0.08);
                physics->SetCollider(0.276,1);
            }else{
                SetSprite("assets/img/belfreefallingtest3.png",4,0.04);
                physics->SetCollider(0.276,0.8);
            }
            swordattack->Restart();
        }
    }
    //HANDLES WHEN TO STOP THE ATTACK
    if((swordattack->Started()) && (swordattack->Get() >= attacktiming) && ((input->IsKeyDown(SDLK_d) || input->IsKeyDown(SDLK_a) || input->IsKeyDown(SDLK_SPACE)))){
        if(!physics->IsGrounded()){
            speed.x = 0;
        }
        for(int i = 0;i < nextattack.size();i++){
            nextattack.pop();
        }
        swordattack->Restart();
    }
}

void Player::XMovement(float dt){
    //Handles input and acceleration
    if( falling || (hittheground->Started())  || (swordattack->Started())  || (jumpanimation->Started())){
        running = false;
    }

    if(input->IsKeyDown(SDLK_d) && !input->IsKeyDown(SDLK_a)){
        if(playersprite->IsFlipped() && (!swordattack->Started())){
            playersprite->Flip();
            speed.x = 0;
            running = false;
        }
        if((running == false) && (physics->IsGrounded()) && (!hittheground->Started())  && (!swordattack->Started())  && (!jumpanimation->Started())){
            SetSprite("assets/img/belwalktest4.png",14,0.04);
            physics->SetCollider(0.184,1);
            running = true;
        }
        physics->PerformXAcceleration(true,aspeed,maxspeed,despeed,dt);
    }
    if(input->IsKeyDown(SDLK_a) && !input->IsKeyDown(SDLK_d)){
        if(!playersprite->IsFlipped() && (!swordattack->Started())){
            playersprite->Flip();
            speed.x = 0;
            running = false;
        }
        if((running == false) && (physics->IsGrounded()) && (!hittheground->Started())  && (!swordattack->Started()) && (!jumpanimation->Started())){
            SetSprite("assets/img/belwalktest4.png",14,0.04);
            physics->SetCollider(0.184,1);
            running = true;
        }
        physics->PerformXAcceleration(false,aspeed,maxspeed,despeed,dt);
    }
    if(input->IsKeyDown(SDLK_a) && input->IsKeyDown(SDLK_d) && (!falling) && (!hittheground->Started())  && (!swordattack->Started())  && (!jumpanimation->Started())){
            physics->PerformXDeceleration(despeed,dt);
            if(running == true){
                SetSprite("assets/img/belstoptest2.png",2,0.04,false);
                physics->SetCollider(0.184,1);
                runningstoptimer->Delay(dt);
                running = false;
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
        running = false;
        if( falling  || (swordattack->Started()) || (jumpanimation->Started())){
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
    if((physics->IsGrounded()) && (falling == true)){
        falling = false;
        if(!swordattack->Started() && !hittheground->Started()){
            SetSprite("assets/img/belhitthegroundtest4.png",4,0.04,false);
            physics->SetCollider(0.276,1);

            Rect collider = physics->GetCollider()->box;
            Vec2 smoke1 = Vec2(collider.x + collider.w/2,collider.y + collider.h - 20);
            SpriteEffect("assets/img/smoketest.png",5,0.05,0.25,smoke1);
            hittheground->Delay(dt);
        }
    }else if((!physics->IsGrounded()) && (speed.y > 0) && (!swordattack->Started())){
        if(physics->distright == 0){
            Rect collider = physics->GetCollider()->box;
            Vec2 smoke1 = Vec2(collider.x + collider.w,collider.y);
            SpriteEffect("assets/img/sparktest.png",4,0.02,0.08,smoke1);
        }
        if(physics->distleft == 0){
            Rect collider = physics->GetCollider()->box;
            Vec2 smoke1 = Vec2(collider.x,collider.y);
            SpriteEffect("assets/img/sparktest.png",4,0.02,0.08,smoke1);
        }
    }

    if(hittheground->Started()){
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
                SetSprite("assets/img/beljumptest4.png",15,0.04,false);
                physics->SetCollider(0.276,0.88888888888);
                jumpanimation->Delay(dt);
                jumpsquat->Delay(dt);
            }else if(physics->distright == 0){
                Rect collider = physics->GetCollider()->box;
                Vec2 smoke1 = Vec2(collider.x + collider.w,collider.y);
                Vec2 smoke2 = Vec2(collider.x + collider.w,collider.y + collider.h);
                SpriteEffect("assets/img/sparktest.png",4,0.02,0.08,smoke1);
                SpriteEffect("assets/img/sparktest.png",4,0.02,0.08,smoke2);
                speed.y = ajump;
                speed.x = -awalljump;
            }else if(physics->distleft == 0){
                Rect collider = physics->GetCollider()->box;
                Vec2 smoke1 = Vec2(collider.x,collider.y);
                Vec2 smoke2 = Vec2(collider.x,collider.y + collider.h);
                SpriteEffect("assets/img/sparktest.png",4,0.02,0.08,smoke1);
                SpriteEffect("assets/img/sparktest.png",4,0.02,0.08,smoke2);
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
            Rect collider = physics->GetCollider()->box;
            Vec2 smoke1 = Vec2(collider.x + collider.w/2,collider.y + collider.h - 20);
            SpriteEffect("assets/img/smoketest.png",5,0.05,0.25,smoke1);
            jumpsquat->Restart();
        }
    }
    if(jumpanimation->Started()){
        jumpanimation->Update(dt);
        if(physics->IsGrounded() && (speed.y > 0)){
            falling = true;
            jumpanimation->Restart();
        }
        if(jumpanimation->Get() >= 0.6){
            jumpanimation->Restart();
        }
    }

    //Handles when it is falling
    if((!physics->IsGrounded()) && (speed.y > 0) && (falling == false) && (!hittheground->Started()) && (!jumpanimation->Started()) && (!damagetimer->Started()) && (!swordattack->Started())){
        SetSprite("assets/img/belfreefallingtest3.png",4,0.04);
        physics->SetCollider(0.276,0.8);
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
    if(!damagetimer->Started()){
        SetSprite("assets/img/beldamagetest.png",7,0.03,false);
        damagetimer->Delay(0);
    }
}

void Player::HealPlayer(int heal){
    hp += heal;
}

void Player::KnockBack(Rect hitbox,Vec2 knockback){
    physics->KnockBack(hitbox,knockback);
}

void Player::SpriteEffect(std::string file,int frames,float frametime,float duration,Vec2 point){
    GameObject *effectObj = new GameObject();
    Sprite *effect = new Sprite(*effectObj,file,frames,frametime,duration,false);
    effectObj->box.x = point.x - effectObj->box.w/2;
    effectObj->box.y = point.y - effectObj->box.h/2;
    effectObj->AddComponent(effect);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
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
                    KnockBack(hitbox->GetOwner()->box,hitbox->GetKnockBack());
                    DamagePlayer(hitbox->GetDamage());
                    invincibilitytimer->Delay(0);
                }
            }
        }
        else if(component2){
            Component *collidercomponent = other.GetComponent("Collider");
            Collider *collider = dynamic_cast<Collider*>(collidercomponent);
            KnockBack(collider->box,Vec2(400,400));
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