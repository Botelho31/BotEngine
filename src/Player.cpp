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
#include "../include/DeadBody.h"
#include "../include/Spike.h"

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

    hp = PLAYERHP;
    invencible = false;
    invincibilitytimer = new Timer();
    damagetimer = new Timer();
    player = this;

    idletimer = new Timer();
    idle = false;

    runningstoptimer = new Timer();
    running = false;

    input =  &(InputManager::GetInstance());
    this->physics = new Physics(associated,&speed);
    associated.AddComponent(physics);

    std::vector<std::string> spritefile;
    spritefile.push_back("assets/img/info/player.txt");
    spritefile.push_back("assets/img/info/effects.txt");
    this->spritefiles = GameData::GetSpritesFiles(spritefile);
    Sprite *player =  new Sprite(associated,spritefiles["idle"],64,0.04);
    this->playersprite = player;
    associated.AddComponent(player);
    physics->SetCollider(0.4,0.995,0,0);
}

Player::~Player(){
    player = nullptr;
    playersprite = nullptr;
    input = nullptr;
    physics = nullptr;
    delete idletimer;
    delete jumpsquat;
    delete hittheground;
    delete invincibilitytimer;
    delete damagetimer;
    delete swordattack;
    delete delayedboosttimer;
    delete jumpanimation;
    delete runningstoptimer;
}

void Player::Start(){
}

void Player::Update(float dt){
    #ifdef DEBUG
        if(input->IsKeyDown(SDLK_MINUS)){
            physics->PrintValues("Player");
        }
        if(input->KeyPress(SDLK_1)){
            if(invencible){
                SetInvincibility(false);
            }else{
                SetInvincibility(true);
            }
        }
    #endif

    AttackHandle(dt);//HANDLING ATTACK
    IdleHandle(dt);//IDLE HANDLING
    XMovement(dt); //X MOVEMENT
    YMovement(dt); //Y MOVEMENT

    if(damagetimer->Started()){
        damagetimer->Update(dt);
        if((hittheground->Started())  || (swordattack->Started()) || (jumpanimation->Started())){
            damagetimer->Restart();
        }
        if(damagetimer->Get() > 0.21){
            damagetimer->Restart();
            if(physics->IsGrounded()){
                if(speed.x == 0){
                    SetSprite(spritefiles["idle"],64,0.04);
                }else{
                    running = true;
                    SetSprite(spritefiles["walking"],14,0.04);
                }
            }
            else{
                falling = true;
                SetSprite(spritefiles["falling"],4,0.04);
            }
        }
    }
    if(invincibilitytimer->Started()){
        invincibilitytimer->Update(dt);
        playersprite->SetAlpha(200);
        if(invincibilitytimer->Get() >= 1){
            invincibilitytimer->Restart();
            playersprite->SetAlpha(255);
        }
    }

    if((hp <= 0) && (GameData::playerAlive)){
        KillPlayer();
    }
}

void Player::InstanceHitbox(){
    Collider *collider = physics->GetCollider();
    Vec2 vector = Vec2(swordradius,0).GetRotated(player->swordarc) + Vec2(collider->box.x + collider->box.w/2,collider->box.y + collider->box.h/2);
    Rect hitbox = Rect(vector.x - 50,vector.y - 30,100,60);
    GameObject *swordObj = new GameObject();
    std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    HitBox *swordhitbox = new HitBox(*swordObj,hitbox,owner,0,PLAYERSWORDDAMAGE,attacktiming - delayedboost,(attacktiming - delayedboost),true,false,true,{300,100},this,0.04);
    swordhitbox->SetFunction(SwordHitbox);
    swordObj->AddComponent(swordhitbox);
    Game::GetInstance().GetCurrentState().AddObject(swordObj);
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

void Player::ProjectileHitbox(GameObject& hitbox,GameObject& owner,float dt){
    Physics *physics = hitbox.GetPhysics();

    if(owner.box.GetCenter().x < physics->GetCollider()->box.GetCenter().x){
        physics->PerformXAcceleration(true,2000,1000,100,dt);
    }else{
        physics->PerformXAcceleration(false,2000,1000,100,dt);
    }
    physics->PerformXMovement(dt);
    if((physics->IsLeft() || physics->IsRight()) && !hitbox.IsDead()){
        hitbox.RequestDelete();
    }
}

void Player::InstanceProjectileHitbox(){
    Vec2 start = Vec2(associated.box.x + associated.box.w,associated.box.y + associated.box.h/2);
    GameObject *projectileObj = new GameObject();
    Sprite *projectilesprite = new Sprite(*projectileObj,spritefiles["projectileattack"],10,0.04);
    projectileObj->AddComponent(projectilesprite);
    std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    HitBox *projectilehitbox = new HitBox(*projectileObj,{0,0,0,0},owner,0,PLAYERPROJECTILEDAMAGE,5,0.5,true,false,true,{300,100},this,0.04);

    //Differentiates starting point based on where player is turned at moment of instance
    if(playersprite->IsFlipped()){
        start = Vec2(associated.box.x,associated.box.y + associated.box.h/2);
        projectilesprite->Flip();
    }
    projectilehitbox->GetCollider()->SetScale({0.8,0.5});
    projectileObj->box.SetCenter(start);


    projectilehitbox->SetFunction(ProjectileHitbox);
    projectileObj->AddComponent(projectilehitbox);
    Game::GetInstance().GetCurrentState().AddObject(projectileObj);
}

void Player::AttackHandle(float dt){
    //HANDLING ATTACK
    if(input->KeyPress(SDLK_e)){    //TESTING SWORD ON E
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
        }else if(nextattack.back() == 3){
            if(nextattack.size() < 2){
                nextattack.push(1);
            }
        }
    }
    if(input->KeyPress(SDLK_q)){
        if(nextattack.empty()){
            nextattack.push(3);
        }
    }
    //PERFORMS THE NEXT ATTACK IN QUEUE
    if((!nextattack.empty()) && (!swordattack->Started())){
        if(nextattack.front() == 1){
            float frametime = 0.03;
            SetSprite(spritefiles["attacking1"],22,frametime,false);
            swordradius = 100;
            boost = 500;
            delayedboost = frametime * 5;
            attacktiming = frametime * 10;
            endofattack = frametime * 25;
            if(playersprite->IsFlipped()){
                player->asword = -((PI * 0.35)/(attacktiming - delayedboost));
                player->swordarc =  -PI + 0.5;
                player->aswordangle = -70;

            }else{
                player->asword = ((PI * 0.35)/(attacktiming - delayedboost));
                player->swordarc =  -0.5;
                player->aswordangle = 70;
            }
            swordattack->Delay(dt);
            delayedboosttimer->Delay(dt);
        }
        if(nextattack.front() == 2){
            float frametime = 0.03;
            SetSprite(spritefiles["attacking2"],22,frametime,false);
            swordradius = 90;
            boost = 500;
            delayedboost = frametime * 5;
            attacktiming = frametime * 10;
            endofattack = frametime * 25;
            if(playersprite->IsFlipped()){
                player->asword= ((PI * 0.35)/(attacktiming - delayedboost));
                player->swordarc =  PI - 0.5;
                player->aswordangle = 70;

            }else{
                player->asword = -((PI * 0.35)/(attacktiming - delayedboost));
                player->swordarc =  0.5;
                player->aswordangle = -70;
            }
            swordattack->Delay(dt);
            delayedboosttimer->Delay(dt);
        }
        if(nextattack.front() == 3){
            if(GameData::savePlayerMana >= PLAYERMANAPERPROJECTILE){
                GameData::savePlayerMana -= PLAYERMANAPERPROJECTILE;
                float frametime = 0.03;
                SetSprite(spritefiles["attacking3"],31,frametime,false);
                swordradius = 90;
                boost = 700;
                delayedboost = frametime * 20;
                attacktiming = frametime * 30;
                endofattack = frametime * 35;
                if(playersprite->IsFlipped()){
                    player->asword= ((PI * 0.35)/(attacktiming - delayedboost));
                    player->swordarc =  PI - 0.5;
                    player->aswordangle = 70;

                }else{
                    player->asword = -((PI * 0.35)/(attacktiming - delayedboost));
                    player->swordarc =  0.5;
                    player->aswordangle = -70;
                }
                swordattack->Delay(dt);
                delayedboosttimer->Delay(dt);
            }else{
                nextattack.pop();
            }
        }
        if(!physics->IsGrounded() && (!physics->IsColliding(physics->GetCollider()->box.Added(0,0,0,100)))){
            speed.y = -350;
        }
    }
    //HANDLES START OF HITBOX AND BOOST FROM ATTACK
    if(delayedboosttimer->Started()){
        delayedboosttimer->Update(dt);
        if(delayedboosttimer->Get() >= delayedboost){
            if(nextattack.front() == 3){
                InstanceProjectileHitbox();
            }else{
                InstanceHitbox();
            }
            if(physics->IsGrounded()){
                if(playersprite->IsFlipped()){
                    speed.x = -boost;
                }else{
                    speed.x = boost;
                }
            }
            delayedboosttimer->Restart();
        }
    }
    //HANDLES THE TIMING OF THE ATTACKS
    if(swordattack->Started()){
        swordattack->Update(dt);
        physics->PerformXDeceleration(3000,dt);
        if((swordattack->Get() >= attacktiming) && (nextattack.size() > 1)){
            nextattack.pop();
            speed.x = 0;
            swordattack->Restart();
        }
        if(swordattack->Get() >= endofattack){
            speed.x = 0;
            nextattack.pop();
            if(physics->IsGrounded()){
                SetSprite(spritefiles["idle"],64,0.04);
            }else{
                SetSprite(spritefiles["falling"],4,0.04);
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

    //Smoke on running printign
    if(running){
        if(playersprite->IsFlipped()){
            Collider *collider = physics->GetCollider();
            Vec2 smoke1 = Vec2(collider->box.x + collider->box.w,collider->box.y + collider->box.h).Added(25,-25);
            if(smoke1.GetDistance(cachepoint.x,cachepoint.y) >= 150){
                SpriteEffect(spritefiles["smoke2"],6,0.1,0.6,smoke1);
                cachepoint = smoke1;
            }
        }else{
            Collider *collider = physics->GetCollider();
            Vec2 smoke1 = Vec2(collider->box.x,collider->box.y + collider->box.h).Added(-25,-25);
            if(smoke1.GetDistance(cachepoint.x,cachepoint.y) >= 150){
                SpriteEffect(spritefiles["smoke2"],6,0.1,0.6,smoke1);
                cachepoint = smoke1;
            }
        }
    }

    if(input->IsKeyDown(SDLK_d) && !input->IsKeyDown(SDLK_a)){
        if(playersprite->IsFlipped() && (!swordattack->Started())){
            playersprite->Flip();
            speed.x = 0;
            running = false;
        }
        if((running == false) && (physics->IsGrounded()) && (!hittheground->Started())  && (!swordattack->Started())  && (!jumpanimation->Started())){
            SetSprite(spritefiles["walking"],14,0.04);
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
            SetSprite(spritefiles["walking"],14,0.04);
            running = true;
        }
        physics->PerformXAcceleration(false,aspeed,maxspeed,despeed,dt);
    }
    if(input->IsKeyDown(SDLK_a) && input->IsKeyDown(SDLK_d) && (!falling) && (!hittheground->Started())  && (!swordattack->Started())  && (!jumpanimation->Started())){
            physics->PerformXDeceleration(despeed,dt);
            if(running == true){
                SetSprite(spritefiles["walkingstop"],2,0.04,false);
                runningstoptimer->Delay(dt);
                running = false;
            }
    }

    if(((input->IsKeyDown(SDLK_a) == false) && (input->IsKeyDown(SDLK_d) == false)) && (physics->IsGrounded())){
        physics->PerformXDeceleration(despeed,dt);
        if(running == true){
            SetSprite(spritefiles["walkingstop"],2,0.04,false);
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
            SetSprite(spritefiles["idle"],64,0.04);
            runningstoptimer->Restart();
        }
    }

    physics->PerformXMovement(dt);//Perfoms Movement if Allowed
}

void Player::YMovement(float dt){

    //Handles when it hits the ground
    if((physics->IsGrounded()) && falling){
        falling = false;
        if(!swordattack->Started() && !hittheground->Started()){
            SetSprite(spritefiles["hittheground"],4,0.04,false,{0,0});

            Rect collider = physics->GetCollider()->box;
            Vec2 smoke1 = Vec2(collider.x + collider.w/2,collider.y + collider.h - 20);
            SpriteEffect(spritefiles["smoke1"],5,0.05,0.25,smoke1);
            hittheground->Delay(dt);
        }
    }else if((!physics->IsGrounded()) && (speed.y > 0) && (!swordattack->Started())){
        if(physics->IsRight()){
            Rect collider = physics->GetCollider()->box;
            Vec2 smoke1 = Vec2(collider.x + collider.w,collider.y);
            if((smoke1.GetDistance(cachepoint.x,cachepoint.y) >= 30) && physics->IsColliding(Rect(smoke1.Added(1,0),1,1))){
                SpriteEffect(spritefiles["smoke2"],6,0.05,0.25,smoke1);
                cachepoint = smoke1;
            }
        }
        if(physics->IsLeft()){
            Rect collider = physics->GetCollider()->box;
            Vec2 smoke1 = Vec2(collider.x,collider.y);
            if((smoke1.GetDistance(cachepoint.x,cachepoint.y) >= 30) && physics->IsColliding(Rect(smoke1.Added(-1,0),1,1))){
                SpriteEffect(spritefiles["smoke2"],6,0.05,0.25,smoke1);
                cachepoint = smoke1;
            }
        }
    }

    if(hittheground->Started()){
        hittheground->Update(dt);
        if(hittheground->Get() >= 0.12){
            if(!swordattack->Started()){
                SetSprite(spritefiles["idle"],64,0.04);
            }
            hittheground->Restart();
        }
    }

    //Handles Jump input and acceleration
    if((input->KeyPress(SDLK_SPACE)) && (!hittheground->Started())){
        if(!swordattack->Started()){
            if(physics->IsGrounded()){
                SetSprite(spritefiles["jumping"],15,0.04,false);
                jumpanimation->Delay(dt);
                jumpsquat->Delay(dt);
            }else if(physics->IsRight()){
                Rect collider = physics->GetCollider()->box;
                Vec2 smoke1 = Vec2(collider.x + collider.w,collider.y);
                Vec2 smoke2 = Vec2(collider.x + collider.w,collider.y + collider.h);
                if(physics->IsColliding(Rect(smoke1.Added(1,0),1,1))){
                    SpriteEffect(spritefiles["smoke2"],6,0.1,0.6,smoke1);
                }
                if(physics->IsColliding(Rect(smoke2.Added(1,0),1,1))){
                    SpriteEffect(spritefiles["smoke2"],6,0.1,0.6,smoke2);
                }
                speed.y = ajump;
                speed.x = -awalljump;
            }else if(physics->IsLeft()){
                Rect collider = physics->GetCollider()->box;
                Vec2 smoke1 = Vec2(collider.x,collider.y);
                Vec2 smoke2 = Vec2(collider.x,collider.y + collider.h);
                if(physics->IsColliding(Rect(smoke1.Added(-1,0),1,1))){
                    SpriteEffect(spritefiles["smoke2"],6,0.1,0.6,smoke1);
                }
                if(physics->IsColliding(Rect(smoke2.Added(-1,0),1,1))){
                    SpriteEffect(spritefiles["smoke2"],6,0.1,0.6,smoke2);
                }
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
            if(physics->IsGrounded()){
                Rect collider = physics->GetCollider()->box;
                Vec2 smoke1 = Vec2(collider.x + collider.w/2,collider.y + collider.h - 20);
                SpriteEffect(spritefiles["smoke1"],5,0.05,0.25,smoke1);
            }
            jumpsquat->Restart();
        }
    }
    if(jumpanimation->Started()){
        jumpanimation->Update(dt);
        if((physics->IsGrounded() && (speed.y >= 0)) && (!jumpsquat->Started())){
            falling = true;
            jumpanimation->Restart();
        }
        if(jumpanimation->Get() >= 0.6){
            jumpanimation->Restart();
            if(!physics->IsGrounded() && !swordattack->Started()){
                falling = true;
                SetSprite(spritefiles["falling"],4,0.04);
            }
        }
    }

    //Handles when it is falling
    if((!physics->IsGrounded()) && (speed.y > 0) && (physics->DistanceTo(physics->GetCollider()->box,0,1,11) > 10) && (falling == false) && (!hittheground->Started()) && (!jumpanimation->Started()) && (!damagetimer->Started()) && (!swordattack->Started())){
        SetSprite(spritefiles["falling"],4,0.04);
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
            idle = true;
        }
    }else{
        idle = false;
        idletimer->Restart();
    }
}

void Player::DamagePlayer(int damage){
    hp -= damage;
    if(hp < 0){
        hp = 0;
    }
    if(!damagetimer->Started() && !swordattack->Started()){
        SetSprite(spritefiles["damage"],7,0.03,false);
        damagetimer->Delay(0);
    }
}

void Player::HealPlayer(int heal){
    hp += heal;
}

void Player::KillPlayer(){
    Camera::UnFollow();
    GameObject *deadObj = new GameObject();
    Sprite *deadsprite = new Sprite(*deadObj,spritefiles["dead"],17,0.06,0,false);
    int xoffset = -40;
    if(playersprite->IsFlipped()){
        deadsprite->Flip();
        xoffset = 40;
    }
    DeadBody *deadbody = new DeadBody(*deadObj,speed,deadsprite,Vec2(0.5,0.2),Vec2(-xoffset,70),true);
    deadObj->AddComponent(deadbody);
    deadObj->box.SetCenter(associated.box.GetCenter());
    Game::GetInstance().GetCurrentState().AddObject(deadObj);
    Camera::Follow(deadObj);
    GameObject *eventObj = new GameObject();
    GameData::playerAlive = false;

    Event *deathevent = new Event(*eventObj,Event::PLAYERDEATH,2.5);
    while(!GameData::events.empty()){
        GameData::events.pop();
    }
    GameData::events.push(deathevent);
    SetSprite(spritefiles["transparent"]);
    SetInvincibility(true);
    KeepStill(true);
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

void Player::Reset(Vec2 speed){
    SetSpeed(speed);
    SetHealth(PLAYERHP);
    swordattack->Restart();
    delayedboosttimer->Restart();
    invincibilitytimer->Restart();
    damagetimer->Restart();
    jumpsquat->Restart();
    hittheground->Restart();
    jumpanimation->Restart();
    runningstoptimer->Restart();
    idletimer->Restart();
    falling = false;
    idle = false;
    invencible = false;
    running = false;
    while(!nextattack.empty()){
        nextattack.pop();
    }
    if(speed.x > 0){
        if(playersprite->IsFlipped()){
            playersprite->Flip();
        }
    }else{
        if(!playersprite->IsFlipped()){
            playersprite->Flip();
        }
    }
    if(physics->IsGrounded()){
        speed.y = 0;
        if(speed.x != 0){
            running = true;
            if(speed.x > 0){
                speed.x = 600;
            }else{
                speed.x = -600;
            }
            SetSprite(spritefiles["walking"],14,0.04);
        }else{
            SetSprite(spritefiles["idle"],64,0.04);
        }
    }
    else{
        falling = true;
        SetSprite(spritefiles["falling"],4,0.04);
    }
}

void Player::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    playersprite->SetAlpha(255);
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    playersprite->SetFrameCount(framecount);
    playersprite->SetFrameTime(frametime);
    playersprite->SetRepeat(repeat);
    playersprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
    physics->GetCollider()->UpdateScale();
}

void Player::PlaySound(std::string file,int times){
    Component *soundcomp = associated.GetComponent("Sound");
    if(soundcomp){
        Sound *sound = dynamic_cast<Sound*>(soundcomp);
        sound->Open(file);
        sound->Play(times);
    }else{
        Sound *sound = new Sound(associated,file);
        sound->Play(times);
    }
}

void Player::StopSound(){
    Component *soundcomp = associated.GetComponent("Sound");
    if(soundcomp){
        Sound *sound = dynamic_cast<Sound*>(soundcomp);
        if(sound->IsPlaying()){
            sound->Stop();
        }
    }
}

void Player::SetSpeed(Vec2 speed){
    this->speed.x = speed.x;
    this->speed.y = speed.y;
}

void Player::SetHealth(int hp){
    this->hp = hp;
}

void Player::KeepStill(bool freeze,float time){
    this->playersprite->KeepStill(freeze,time);
    Component::KeepStill(freeze,time);
}

void Player::SetInvincibility(bool Invencible){
    this->invencible = Invencible;
}

void Player::MovePlayer(float x,float y,bool keepMomentum){
    if(!keepMomentum){
        speed.y = 0;
        speed.x = 0;
    }
    physics->GetCollider()->box.x = x - physics->GetCollider()->box.w/2;
    physics->GetCollider()->box.y = y - physics->GetCollider()->box.h/2;
    physics->GetCollider()->UpdateAssociated();
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
    if(!invincibilitytimer->Started() && !invencible){
        Component *component1 = other.GetComponent("HitBox");
        Component *component2 = other.GetComponent("Minion");
        Component *component3 = other.GetComponent("Spike");
        Component *component4 = other.GetComponent("Spit");
        if(component1){
            HitBox *hitbox = dynamic_cast<HitBox*>(component1);
            if(hitbox->GetOwner()){
                if(hitbox->HitPlayer() && (hitbox->GetDamage() > 0)){
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
        }else if(component3){
            Component *collidercomponent = other.GetComponent("Collider");
            Collider *collider = dynamic_cast<Collider*>(collidercomponent);
            Spike *spike = dynamic_cast<Spike*>(component3);
            KnockBack(collider->box,spike->GetKnockback());
            DamagePlayer(spike->GetDamage());
            invincibilitytimer->Delay(0);
        }
        else if(component4){
            Component *collidercomponent = other.GetComponent("Collider");
            Collider *collider = dynamic_cast<Collider*>(collidercomponent);
            KnockBack(collider->box,Vec2(300,0));
            DamagePlayer(10);
            invincibilitytimer->Delay(0);
        }
    }

    Component *component = other.GetComponent("Event");
    if(component){
        Event *event = dynamic_cast<Event*>(component);
        if(!event->IsProcessing() && GameData::playerAlive){
            event->SetProcessing(true);
            GameData::events.push(event);
        }
    }
}

Physics* Player::GetPhysics(){
    return physics;
}

Vec2 Player::GetPosition(){
    return physics->GetCollider()->box.GetCenter();
}

float Player::GetPlayerAngle(){
    return ToPI(associated.angleDeg);
}

Vec2 Player::GetSpeed(){
    return Vec2(speed.x,speed.y);
}

int Player::GetLife(){
    return hp;
}

GameObject* Player::GetAssociated(){
    return &associated;
}