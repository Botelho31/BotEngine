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

    hp = 60;
    attackrange = 150;
    sightrange = 500;
    damageCooldown = 0;
    invincibilitytimer =  new Timer();

    idletimer = new Timer();
    idle = false;

    state = IDLE;
    this->physics = new Physics(&associated,&speed);

    this->attacktimer = new Timer();

    Sprite *minion =  new Sprite(associated,"assets/img/minionidletest.png",32,0.08);
    this->minionsprite = minion;
    associated.AddComponent(minion);
    physics->SetCollider(0.5,0.65,0,33);
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
    float distanceToPlayer = sightrange;
    Vec2 player = Vec2(0,0);
    if(Player::player){
        distanceToPlayer = physics->DistanceTo(GetPosition(),Player::player->GetPosition(),sightrange);
        player = Player::player->GetPosition();
    }
    XMovement(dt);
    YMovement(dt);

    if(state == IDLE){
        if(distanceToPlayer < sightrange){
            state = CHASING;
        }
        IdleHandle(dt);
        physics->PerformXDeceleration(despeed,dt);
    }
    if(state == CHASING){
        if(distanceToPlayer >= sightrange){
            state = IDLE;
            SetSprite("assets/img/minionidletest.png",32,0.08);
        }else if(distanceToPlayer <= attackrange){
            state = ATTACKING;
        }
        if(player.x < GetPosition().x){
            if(!minionsprite->IsFlipped()){
                minionsprite->Flip();
            }
            physics->PerformXAcceleration(false,aspeed,maxspeed,despeed,dt);
        }else{
            if(minionsprite->IsFlipped()){
                minionsprite->Flip();
            }
            physics->PerformXAcceleration(true,aspeed,maxspeed,despeed,dt);
        }
    }
    if(state == ATTACKING){
        if(!invincibilitytimer->Started()){
            if((!attacktimer->Started())){
                speed.x = 0;
                Rect hitbox;
                if(player.x < GetPosition().x){
                    hitbox = Rect(collider->box.x - collider->box.w,collider->box.y,collider->box.w,collider->box.h);
                }else{
                    hitbox = Rect(collider->box.x + collider->box.w,collider->box.y,collider->box.w,collider->box.h);
                }
                GameObject *hitboxObj = new GameObject();

                std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
                HitBox *minionhitbox = new HitBox(*hitboxObj,hitbox,owner,0,30,1,1,false,true,false,{400,100},this);
                minionhitbox->SetFunction(BiteHitbox);
                hitboxObj->AddComponent(minionhitbox);
                Game::GetInstance().GetCurrentState().AddObject(hitboxObj);
                attacktimer->Delay(dt);
            }else{
                attacktimer->Update(dt);
            }
            if(attacktimer->Get() >= 1){
                attacktimer->Restart();
                if((distanceToPlayer >= attackrange) && (distanceToPlayer < sightrange)){
                    state = CHASING;
                }else if(distanceToPlayer >= sightrange){
                    state = IDLE;
                    SetSprite("assets/img/minionidletest.png",32,0.08);
                    
                }
            }
        }else{
            if((distanceToPlayer >= attackrange) && (distanceToPlayer < sightrange)){
                state = CHASING;
            }else if(distanceToPlayer >= sightrange){
                state = IDLE;
                SetSprite("assets/img/minionidletest.png",32,0.08);
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

void Minion::DamageMinion(int damage){
    hp -= damage;
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
    physics->PerformXMovement(dt);//Perfoms Movement if Allowed
}
void Minion::YMovement(float dt){
    physics->PerformYMovement(dt);//Performs movement if it is allowed
    physics->PerformGravity(gravspeed,dt); // Gravity
}
void Minion::IdleHandle(float dt){
    if((speed.x == 0) && (speed.y == 0) && (state == IDLE)){
        idletimer->Update(dt);
        if((idletimer->Get() > 2) && (idle == false)){
            idle = true;
        }
    }else{
        idle = false;
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
    if(!invincibilitytimer->Started()){
        Component *component1 = other.GetComponent("HitBox");
        Component *component2 = other.GetComponent("Minion");
        if(component1){
            HitBox *hitbox = dynamic_cast<HitBox*>(component1);
            if((hitbox)  && hitbox->HitEnemy()){
                physics->KnockBack(hitbox->GetOwner()->box,hitbox->GetKnockBack());
                DamageMinion(hitbox->GetDamage());
                damageCooldown = hitbox->GetDamageCooldown();
                invincibilitytimer->Delay(0);
            }
        }
    }
}

Vec2 Minion::GetPosition(){
    return Vec2(associated.box.x + associated.box.w/2,associated.box.y + associated.box.h/2);
}
