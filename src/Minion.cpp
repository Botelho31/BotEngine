#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Player.h"
#include "../include/HitBox.h"
#include "../include/Camera.h"
#include "../include/WindowEffects.h"
#include "../include/DeadBody.h"
#include "../include/GameData.h"

Minion::Minion(GameObject& associated,minionState startingState) : Component(associated){
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    gravspeed = 2000;
    hittheground = new Timer();

    hp = MINIONHP;
    attackrange = 150;
    sightrange = 500;
    damageCooldown = 0;
    invincibilitytimer =  new Timer();
    damagetimer = new Timer();
    attackdelay = new Timer();

    idletimer = new Timer();
    idle = false;

    state = startingState;
    this->physics = new Physics(associated,&speed,false,true);
    associated.AddComponent(physics);

    this->attacktimer = new Timer();
    sightangle = 0;
    
    std::vector<std::string> spritefile;
    spritefile.push_back("assets/img/info/minion.txt");
    spritefile.push_back("assets/img/info/effects.txt");
    spritefiles = GameData::GetSpritesFiles(spritefile);
    Sprite *minion =  new Sprite(associated,spritefiles["idle"],32,0.08);
    this->minionsprite = minion;
    associated.AddComponent(minion);
    physics->SetCollider(0.5,0.65,0,33);

    if(state == FALLINGFROMBOSS){
        physics->GetCollider()->Update(0);
        SetSprite(spritefiles["falling"],18,0.04,true);
    }
}

Minion::~Minion(){
    minionsprite = nullptr;
    physics = nullptr;
    delete idletimer;
    delete hittheground;
    delete invincibilitytimer;
    delete damagetimer;
    delete attacktimer;
    delete attackdelay;
}

void Minion::Start(){

}

void Minion::Update(float dt){
    // physics->Update(dt);
    int distanceToPlayer = sightrange;
    Vec2 player = Vec2(0,0);
    if(Player::player && GameData::playerAlive){
        Vec2 minionpos = GetPosition();
        player = Player::player->GetPosition();
        int distances[] = { floor(minionpos.GetDistance(player.x,player.y)),
                            floor(minionpos.GetDistance(player.x,player.y - 100)),
                            floor(minionpos.GetDistance(player.x,player.y + 100))};
        int dists[] = { physics->SightTo(minionpos,player.Added(0,-100),sightrange),
                        physics->SightTo(minionpos,player,sightrange),
                        physics->SightTo(minionpos,player.Added(0,100),sightrange)};

        int size = sizeof(dists)/sizeof(dists[0]);
        std::sort(dists,dists+size);
        distanceToPlayer = dists[0];

        if(distanceToPlayer < sightrange){
            if(distanceToPlayer == distances[1]){
                sightangle = minionpos.GetAngle(player.x,player.y - 100);
                sightline = physics->GetLineBox(minionpos,player.Added(0,-100),distanceToPlayer);
            }else if(distanceToPlayer == distances[2]){
                sightangle = minionpos.GetAngle(player.x,player.y + 100);
                sightline = physics->GetLineBox(minionpos,player.Added(0,100),distanceToPlayer);
            }else{
                sightangle = minionpos.GetAngle(player.x,player.y);
                sightline = physics->GetLineBox(minionpos,player,distanceToPlayer);
            }
        }else{
            sightline.Transform(minionpos.x,minionpos.y);
            sightline.w = 10;
            sightline.h = 10;
        }
    }
    switch(state){
        case IDLE:
            IdleState(distanceToPlayer,dt);
            break;
        case CHASING:
            ChasingState(distanceToPlayer,dt);
            break;
        case ATTACKING:
            AttackState(distanceToPlayer,dt);
            break;
        case FALLINGFROMBOSS:
            if(physics->IsGrounded()){
                if(!hittheground->Started()){
                    hittheground->Delay(dt);
                }
                physics->PerformXDeceleration(despeed,dt);
            }
            if(hittheground->Started()){
                hittheground->Update(dt);
                if(physics->IsGrounded()){
                    hittheground->Restart();
                    state = IDLE;
                    SetSprite(spritefiles["idle"],32,0.08);
                }
                if(hittheground->Get() > 2){
                    state = IDLE;
                    SetSprite(spritefiles["idle"],32,0.08);
                }
            }
        default:
            break;
    }

    XMovement(dt);
    YMovement(dt);

    if(attackdelay->Started()){
        attackdelay->Update(dt);
        if(attackdelay->Get() > 0.5){
            attackdelay->Restart();
        }
    }
    if(damagetimer->Started()){
        damagetimer->Update(dt);
        if(damagetimer->Get() > 0.20){
            damagetimer->Restart();
            if(state == CHASING){
                SetSprite(spritefiles["walking"],8,0.08);
            }else if(state == IDLE){
                SetSprite(spritefiles["idle"],32,0.08);
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
    if((hp <= 0) || (physics->IsOutofBounds(true))){
        if(!associated.IsDead()){
            if(physics->IsOutofBounds(true)){
                associated.RequestDelete();
            }else{
                KillMinion();
            }
        }
    }
}

void Minion::DamageMinion(int damage){
    hp -= damage;
    if(!damagetimer->Started() && (!attacktimer->Started() && (hp > 0) && state != FALLINGFROMBOSS)){
        SetSprite(spritefiles["damage"],5,0.04);
        damagetimer->Delay(0);
    }else if(hp <= 0){
        KillMinion();
    }
}

void Minion::KillMinion(){
    GameObject *deadObj = new GameObject();
    int animrand = rand() % 2 + 1;
    if(animrand == 1){
        Sprite *deadsprite = new Sprite(*deadObj,spritefiles["deadbehind"],34,0.04,0,false,true);
        int xoffset = -40;
        if(minionsprite->IsFlipped()){
            deadsprite->Flip();
            xoffset = 40;
        }
        DeadBody *deadbody = new DeadBody(*deadObj,speed,deadsprite,Vec2(0.5,0.35),Vec2(xoffset,80),false);
        deadObj->AddComponent(deadbody);
        deadObj->box.SetCenter(associated.box.GetCenter().Added(0,0));
        Game::GetInstance().GetCurrentState().AddObject(deadObj);
        associated.RequestDelete();
    }else{
        Sprite *deadsprite = new Sprite(*deadObj,spritefiles["deadfront"],36,0.04,0,false,true);
        int xoffset = -40;
        if(minionsprite->IsFlipped()){
            deadsprite->Flip();
            xoffset = 40;
        }
        DeadBody *deadbody = new DeadBody(*deadObj,speed,deadsprite,Vec2(0.5,0.35),Vec2(-xoffset,80),false);
        deadObj->AddComponent(deadbody);
        deadObj->box.SetCenter(associated.box.GetCenter().Added(0,0));
        Game::GetInstance().GetCurrentState().AddObject(deadObj);
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
    physics->PerformXMovement(dt);//Perfoms Movement if Allowed
}
void Minion::YMovement(float dt){
    physics->PerformYMovement(dt);//Performs movement if it is allowed
    physics->PerformGravity(gravspeed,dt); // Gravity
}

void Minion::AttackState(float distanceToPlayer,float dt){
    Collider *collider = physics->GetCollider();
    Vec2 player = Player::player->GetPosition();

    if((!attacktimer->Started()) && (!attackdelay->Started())){
        hitboxinstantiated = false;
        if(player.x < GetPosition().x){
            difxpos = false;
            if(!minionsprite->IsFlipped()){
                minionsprite->Flip();
            }
        }else{
            difxpos = true;
            if(minionsprite->IsFlipped()){
                minionsprite->Flip();
            }
        }
        SetSprite(spritefiles["attacking"],27,0.04,false);
        attacktimer->Delay(dt);
    }else if(!attacktimer->Started() && attackdelay->Started()){
        if((distanceToPlayer >= attackrange) && (distanceToPlayer < sightrange)){
            state = CHASING;
            SetSprite(spritefiles["walking"],8,0.08);
        }else{
            state = IDLE;
            SetSprite(spritefiles["idle"],32,0.08);
        }
    }

    if(attacktimer->Started()){
        speed.x = 0;
        attacktimer->Update(dt);
        if((attacktimer->Get() >= 0.56) && (!hitboxinstantiated)){
            hitboxinstantiated = true;
            Rect hitbox;
            if(!difxpos){
                hitbox = Rect(collider->box.x - collider->box.w,collider->box.y,collider->box.w,collider->box.h);
            }else{
                hitbox = Rect(collider->box.x + collider->box.w,collider->box.y,collider->box.w,collider->box.h);
            }
            GameObject *hitboxObj = new GameObject();
            std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
            HitBox *minionhitbox = new HitBox(*hitboxObj,hitbox,owner,0,MINIONDAMAGE,0.44,0.44,false,true,false,{400,100},this);
            minionhitbox->SetFunction(BiteHitbox);
            hitboxObj->AddComponent(minionhitbox);
            Game::GetInstance().GetCurrentState().AddObject(hitboxObj);
        }
        if(attacktimer->Get() >= 1.2){
            attacktimer->Restart();
            attackdelay->Delay(dt);
            if((distanceToPlayer >= attackrange) && (distanceToPlayer < sightrange)){
                state = CHASING;
                SetSprite(spritefiles["walking"],8,0.08);
            }else{
                state = IDLE;
                SetSprite(spritefiles["idle"],32,0.08);
            }
        }
    }
}

void Minion::ChasingState(float distanceToPlayer,float dt){
    Collider *collider = physics->GetCollider();
    Vec2 player = Player::player->GetPosition();

    if((distanceToPlayer >= sightrange) || ((distanceToPlayer - (speed.x * dt)<= attackrange) && (attackdelay->Started()))){
        state = IDLE;
        SetSprite(spritefiles["idle"],32,0.08);
    }else if((distanceToPlayer - (speed.x * dt)<= attackrange) && (!attackdelay->Started())){
        state = ATTACKING;
    }else{
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
}

void Minion::IdleState(float distanceToPlayer,float dt){
    if((distanceToPlayer - (speed.x * dt)<= attackrange) && (attackdelay->Started())){

    }
    else if((distanceToPlayer - (speed.x * dt)<= attackrange) && (!attackdelay->Started())){
        state = ATTACKING;
    }
    else if(distanceToPlayer < sightrange){
        state = CHASING;
        SetSprite(spritefiles["walking"],8,0.08);
    }
    if(state == IDLE){
        IdleHandle(dt);
        physics->PerformXDeceleration(despeed,dt);
    }

}

void Minion::IdleHandle(float dt){
    if((speed.x == 0) && (speed.y == 0)){
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
    physics->GetCollider()->UpdateScale();
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
                if(hitbox->GetOwner()->box.GetCenter().x < GetPosition().x){
                    if(!minionsprite->IsFlipped()){
                        minionsprite->Flip();
                    }
                }else{
                    if(minionsprite->IsFlipped()){
                        minionsprite->Flip();
                    }
                }
                DamageMinion(hitbox->GetDamage());
                damageCooldown = hitbox->GetDamageCooldown();
                invincibilitytimer->Delay(0);
            }
        }
        if(component2){
            physics->KnockBack(other.box,{5,0},true,{600,400});
        }
    }
}

Vec2 Minion::GetPosition(){

    return physics->GetCollider()->box.GetCenter();
}

Minion::minionState Minion::GetState(){
    return state;
}
