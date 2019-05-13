#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Player.h"
#include "../include/HitBox.h"
#include "../include/Camera.h"
#include "../include/WindowEffects.h"

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
    damagetimer = new Timer();

    idletimer = new Timer();
    idle = false;

    state = IDLE;
    this->physics = new Physics(&associated,&speed);

    this->attacktimer = new Timer();

    sightangle = 0;
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
    delete damagetimer;
    delete physics;
    delete attacktimer;
}

void Minion::Start(){

}

void Minion::Update(float dt){
    Collider *collider = physics->GetCollider();
    physics->Update();
    float distanceToPlayer = sightrange;
    Vec2 player = Vec2(0,0);
    if(Player::player){
        Vec2 minionpos = GetPosition();
        player = Player::player->GetPosition();
        float distance = minionpos.GetDistance(player.x,player.y);
        float distance2 = minionpos.GetDistance(player.x,player.y - 100);
        float distance3 = minionpos.GetDistance(player.x,player.y + 100);
        float dists[] = { physics->SightTo(minionpos,player.Added(0,-100),sightrange),
                        physics->SightTo(minionpos,player,sightrange),
                        physics->SightTo(minionpos,player.Added(0,100),sightrange)};
        int size = sizeof(dists)/sizeof(dists[0]);
        std::sort(dists,dists+size);
        distanceToPlayer = dists[0];

        if(distanceToPlayer < sightrange){
            if(distanceToPlayer == floor(distance2)){
                sightangle = minionpos.GetAngle(player.x,player.y - 100);
            }else if(distanceToPlayer == floor(distance3)){
                sightangle = minionpos.GetAngle(player.x,player.y + 100);
            }else{
                sightangle = minionpos.GetAngle(player.x,player.y);
            }
            Vec2 vector = Vec2(distanceToPlayer,0).GetRotated(sightangle) + minionpos;
            sightline = Rect(vector.x - (distanceToPlayer * (((cos(std::fabs(sightangle))) + 1)/2) ),vector.y + (distanceToPlayer/2 * -sin(sightangle)),distanceToPlayer,1);
        }else{
            sightline.Transform(minionpos.x,minionpos.y);
            sightline.w = 10;
            sightline.h = 10;
        }
    }
    XMovement(dt);
    YMovement(dt);

    if(state == IDLE){
        if(distanceToPlayer < sightrange){
            state = CHASING;
            SetSprite("assets/img/minionwalktest.png",8,0.08);
        }
        IdleHandle(dt);
        physics->PerformXDeceleration(despeed,dt);
    }
    if(state == CHASING){
        if(distanceToPlayer >= sightrange){
            state = IDLE;
            SetSprite("assets/img/minionidletest.png",32,0.08);
        }else if(distanceToPlayer - (speed.x * dt)<= attackrange){
            state = ATTACKING;
            SetSprite("assets/img/minionidletest.png",32,0.08);
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
                    SetSprite("assets/img/minionwalktest.png",8,0.08);
                }else if(distanceToPlayer >= sightrange){
                    state = IDLE;
                    SetSprite("assets/img/minionidletest.png",32,0.08);
                    
                }
            }
        }else{
            if((distanceToPlayer >= attackrange) && (distanceToPlayer < sightrange)){
                state = CHASING;
                SetSprite("assets/img/minionwalktest.png",8,0.08);
            }else if(distanceToPlayer >= sightrange){
                state = IDLE;
                SetSprite("assets/img/minionidletest.png",32,0.08);
            }
        }
    }
    if(damagetimer->Started()){
        damagetimer->Update(dt);
        if(damagetimer->Get() > 0.20){
            damagetimer->Restart();
            if(state == IDLE){
                SetSprite("assets/img/minionidletest.png",32,0.08);
            }else if(state == CHASING){
                SetSprite("assets/img/minionwalktest.png",8,0.08);
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
    if(!damagetimer->Started()){
        SetSprite("assets/img/miniondamagetest.png",5,0.04);
        damagetimer->Delay(0);
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
    #ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS) && Player::player){
        WindowEffects::DrawBox(sightline,sightangle,255,0,0);
	}
    #endif // DEBUG
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

Physics* Minion::GetPhysics(){
    return physics;
}

Vec2 Minion::GetPosition(){

    return physics->GetCollider()->box.GetCenter();
}
