#include "../include/HitBox.h"
#include "../include/Player.h"


HitBox::HitBox(GameObject& associated,Rect hitbox,std::weak_ptr<GameObject> owner,double angledeg,int damage,float secondsToSelfDestruct,float damageCooldown,bool disconnected,bool hitPlayer,bool hitEnemy,Vec2 knockback,Component *component,float hitfreezetime) : 
    Component(associated),secondsToSelfDestruct(secondsToSelfDestruct),Move(NULL),owner(owner),damageCooldown(damageCooldown + hitfreezetime),hitPlayer(hitPlayer),hitEnemy(hitEnemy),disconnected(disconnected),damage(damage),knockback(knockback),component(component),hitfreezetime(hitfreezetime){
    associated.box = hitbox;
    associated.angleDeg = angledeg;
    this->selfDestruct = new Timer();
    Vec2 speed;
    this->physics = new Physics(&associated,&speed);
}

HitBox::~HitBox(){
    delete selfDestruct;
    delete physics;
}

void HitBox::SetFunction( void(*NewFunc)(GameObject&,GameObject&,float) ){
    this->Move = NewFunc;
}

void HitBox::Update(float dt){
    this->physics->Update(associated.box,50);
    if(secondsToSelfDestruct > 0){
        selfDestruct->Update(dt);
        damageCooldown -= dt;
        if(selfDestruct->Get() >= secondsToSelfDestruct){
            associated.RequestDelete();
        }
    }
    if(!owner.expired()){
        if(Move != NULL){
            Move(associated,*owner.lock().get(),dt);
        }
        //Part that handles player hitting the wall
        if(owner.lock().get()->GetComponent("Player")){
            Vec2 collisionpoint = physics->GetCollisionPoint(owner.lock().get()->box);
            if((collisionpoint.x != 0) && (collisionpoint.y != 0)){
                HitEffect("assets/img/sparktest.png",4,0.02,0.08,collisionpoint);
                Component *component1 = owner.lock().get()->GetComponent("Player");
                Player *player = dynamic_cast<Player*>(component1);
                // component1->KeepStill(true,1);
                // KeepStill(true,1);
                Rect collisionrect = Rect(collisionpoint.x,collisionpoint.y,0,0);
                Vec2 ownercenter = owner.lock().get()->box.GetCenter();
                float collisiondist = collisionpoint.GetDistance(ownercenter.x,ownercenter.y);
                float wallknockback = 200 + (300 * (30/(collisiondist)));
                // float test = (30/(collisiondist));
                // std::cout << test << std::endl;
                // std::cout << "dist: " << collisiondist << " KB: " << wallknockback << std::endl;
                player->KnockBack(collisionrect,Vec2(wallknockback,0));
            }
        }
    }else{
        associated.RequestDelete();
    }

}

void HitBox::Render(){

}

void HitBox::NotifyCollision(GameObject& other){
    if(!disconnected){
        if(!owner.expired()){
            if(component){
                component->NotifyCollision(other);
            }
        }
    }
    //HANDLES WHEN TO FREEZE THE HIT
    if(component && (hitfreezetime > 0)){
        Component *hitboxcomponent = other.GetComponent("HitBox");
        if(hitPlayer){
            Component *component1 = other.GetComponent("Player");
            if(component1){
                component->KeepStill(true,hitfreezetime);
                KeepStill(true,hitfreezetime);
            }
            if(hitboxcomponent){
                HitBox *hitbox = dynamic_cast<HitBox*>(hitboxcomponent);
                if(hitbox->HitEnemy()){
                    component->KeepStill(true,hitfreezetime);
                    KeepStill(true,hitfreezetime);
                    HitEffect("assets/img/sparktest.png",4,0.04,0.16,associated.box.GetCenter());
                }
            }
        }
        if(hitEnemy){
            Component *component1 = other.GetComponent("Minion");
            if(component1){
                component->KeepStill(true,hitfreezetime);
                KeepStill(true,hitfreezetime);
                component1->KeepStill(true,hitfreezetime);
                HitEffect("assets/img/sparktest.png",4,0.04,0.16,associated.box.GetCenter());
                hitfreezetime = 0;
                knockback.x = 0;
                knockback.y = 0;
            }
            if(hitboxcomponent){
                HitBox *hitbox = dynamic_cast<HitBox*>(hitboxcomponent);
                if(hitbox->HitPlayer()){
                    component->KeepStill(true,hitfreezetime);
                    KeepStill(true,hitfreezetime);
                    HitEffect("assets/img/sparktest.png",4,0.04,0.16,associated.box.GetCenter());
                }
            }
        }
    }
}

bool HitBox::Is(std::string type){
    if(type == "HitBox"){
        return true;
    }else{
        return false;
    }
}

void HitBox::HitEffect(std::string file,int frames,float frametime,float duration,Vec2 point){
    GameObject *effectObj = new GameObject();
    Sprite *effect = new Sprite(*effectObj,file,frames,frametime,duration,false);
    effectObj->box.x = point.x - effectObj->box.w/2;
    effectObj->box.y = point.y - effectObj->box.h/2;
    effectObj->AddComponent(effect);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
}

Collider* HitBox::GetCollider(){
    return physics->GetCollider();
}

std::shared_ptr<GameObject> HitBox::GetOwner(){
    if(!owner.expired()){
        return owner.lock();
    }else{
        return nullptr;
    }
}

Vec2 HitBox::GetKnockBack(){
    return this->knockback;
}

bool HitBox::HitPlayer(){
    return hitPlayer;
}

bool HitBox::HitEnemy(){
    return hitEnemy;
}

float HitBox::GetDamageCooldown(){
    return damageCooldown;
}

int HitBox::GetDamage(){
    return damage;
}

void HitBox::SetSelfDestruct(float secondsToSelfDestruct){
    this->secondsToSelfDestruct = secondsToSelfDestruct;
}

void HitBox::SetDamageValues(int damage,float damageCooldown,Vec2 knockback){
    this->damage = damage;
    this->damageCooldown = damageCooldown;
    this->knockback = knockback;
}

void HitBox::SetHit(bool HitPlayer,bool HitEnemy){
    this->hitPlayer = HitPlayer;
    this->hitEnemy = HitEnemy;
}

