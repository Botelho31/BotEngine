#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Player.h"
#include "../include/HitBox.h"

Minion::Minion(GameObject& associated) : Component(associated){
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    gravspeed = 2000;
    hittheground = new Timer();

    hp = 50;
    damageCooldown = 0;
    invincibilitytimer =  new Timer();

    idletimer = new Timer();
    idle = false;

    state = IDLE;
    this->physics = new Physics(&associated);

    this->attacktimer = new Timer();

    Sprite *minion =  new Sprite(associated,"assets/img/miniontest.png");
    this->minionsprite = minion;
    associated.AddComponent(minion);
    physics->SetCollider(0.5,0.9);
}

Minion::~Minion(){
    minionsprite = nullptr;
    delete idletimer;
    delete hittheground;
    delete invincibilitytimer;
    delete physics;
    delete attacktimer;
}

void Minion::Start(){

}

void Minion::Update(float dt){
    Collider *collider = physics->GetCollider();
    physics->Update(collider->box);
    physics->CorrectDistance();
    float distanceToPlayer = 500;
    Vec2 player = Vec2(0,0);
    if(Player::player){
        distanceToPlayer = physics->DistanceTo(GetPosition(),Player::player->GetPosition(),500);
        player = Player::player->GetPosition();
    }

    XMovement(dt);
    YMovement(dt);
    if(state == IDLE){
        if(distanceToPlayer < 500){
            state = CHASING;
        }
        IdleHandle(dt);
        physics->PerformXDeceleration(&speed,despeed,dt);
    }
    if(state == CHASING){
        if(distanceToPlayer >= 500){
            state = IDLE;
        }else if(distanceToPlayer <= 150){
            state = ATTACKING;
        }
        if(player.x < GetPosition().x){
            if(!minionsprite->IsFlipped()){
                minionsprite->Flip();
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
        }else{
            if(minionsprite->IsFlipped()){
                minionsprite->Flip();
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
    }
    if(state == ATTACKING){
        speed.x = 0;
        if(!attacktimer->Started()){
            Rect hitbox;
            if(player.x < GetPosition().x){
                hitbox = Rect(collider->box.x - collider->box.w,collider->box.y,collider->box.w,collider->box.h);
            }else{
                hitbox = Rect(collider->box.x + collider->box.w,collider->box.y,collider->box.w,collider->box.h);
            }
            GameObject *hitboxObj = new GameObject();

            std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
            HitBox *minionhitbox = new HitBox(*hitboxObj,hitbox,owner,0,30,1,1,false,true,false,{400,400},this);
            minionhitbox->SetFunction(BiteHitbox);
            hitboxObj->AddComponent(minionhitbox);
            Game::GetInstance().GetCurrentState().AddObject(hitboxObj);
            attacktimer->Delay(dt);
        }else{
            attacktimer->Update(dt);
        }
        if(attacktimer->Get() >= 1){
            attacktimer->Restart();
            if((distanceToPlayer >= 100) && (distanceToPlayer < 500)){
                state = CHASING;
            }else if(distanceToPlayer >= 500){
                state = IDLE;
            }
        }
    }
    if(invincibilitytimer->Started()){
        invincibilitytimer->Update(dt);
        if(invincibilitytimer->Get() >= damageCooldown){
            invincibilitytimer->Restart();
            this->damageCooldown = 0;
        }
    }
    if(hp <= 0){
        associated.RequestDelete();
    }
}

void Minion::BiteHitbox(GameObject& hitbox,GameObject& owner,float dt){
    Component *component = owner.GetComponent("Collider");
    Collider *collider = dynamic_cast<Collider*>(component);
    if(hitbox.box.x < collider->box.x){
        hitbox.box.x = collider->box.x - collider->box.w;
    }else{
        hitbox.box.x = collider->box.x + collider->box.w;
    }
    hitbox.box.y = collider->box.y;
}

void Minion::XMovement(float dt){
    physics->PerformXMovement(&speed,dt);//Perfoms Movement if Allowed
}
void Minion::YMovement(float dt){
    physics->PerformYMovement(&speed,dt);//Performs movement if it is allowed
    //GRAVITY
    if(!physics->IsGrounded()){
        if(idle == true){
            idletimer->Restart();
            idle = false;
        }
        speed.y += gravspeed*dt;
    }
}
void Minion::IdleHandle(float dt){
    if((idle == false) && (speed.x == 0) && (speed.y == 0)){
        idletimer->Update(dt);
        if((idletimer->Get() > 2) && (idle == false)){
            idle = true;
        }
    }else{
        idletimer->Restart();
    }
}

void Minion::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    minionsprite->SetFrameCount(framecount);
    minionsprite->SetFrameTime(frametime);
    minionsprite->SetRepeat(repeat);
    minionsprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
}

void Minion::Render(){

}

bool Minion::Is(std::string type){
    if(type == "Minion"){
        return true;
    }else{
        return false;
    }
}

void Minion::NotifyCollision(GameObject& other){
    if(invincibilitytimer->Started()){
    }else{
        Component *component1 = other.GetComponent("HitBox");
        if(component1){
            HitBox *hitbox = dynamic_cast<HitBox*>(component1);
            if((hitbox)  && hitbox->HitEnemy()){
                physics->KnockBack(hitbox->GetOwner()->box,&speed,hitbox->GetKnockBack());
                damageCooldown = hitbox->GetDamageCooldown();
                invincibilitytimer->Update(0);
            }
        }
    }
}

Vec2 Minion::GetPosition(){
    return Vec2(associated.box.x + associated.box.w/2,associated.box.y + associated.box.h/2);
}
