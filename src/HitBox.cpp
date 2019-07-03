#include "../include/HitBox.h"
#include "../include/Player.h"
#include "../include/Minion.h"
#include "../include/GameData.h"
#include "../include/Boss.h"


HitBox::HitBox(GameObject& associated,Rect hitbox,std::weak_ptr<GameObject> owner,double angledeg,int damage,float secondsToSelfDestruct,float damageCooldown,bool disconnected,bool hitPlayer,bool hitEnemy,Vec2 knockback,Component *component,float hitfreezetime) : 
    Component(associated),secondsToSelfDestruct(secondsToSelfDestruct),Move(NULL),owner(owner),damageCooldown(damageCooldown + hitfreezetime),hitPlayer(hitPlayer),hitEnemy(hitEnemy),disconnected(disconnected),damage(damage),knockback(knockback),component(component),hitfreezetime(hitfreezetime){
    if(hitbox == Rect(0,0,0,0)){
    }else{
        associated.box = hitbox;
    }
    associated.angleDeg = angledeg;
    this->selfDestruct = new Timer();
    this->physics = new Physics(associated,&speed,true);
    associated.AddComponent(physics);
    std::vector<std::string> spritefile;
    spritefile.push_back("assets/img/info/effects.txt");
    this->spritefiles =  GameData::GetSpritesFiles(spritefile);
}

HitBox::~HitBox(){
    physics = nullptr;
    delete selfDestruct;
}

void HitBox::SetFunction( void(*NewFunc)(GameObject&,GameObject&,float) ){
    this->Move = NewFunc;
}

void HitBox::Update(float dt){
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
            Vec2 collisionpoint = physics->GetCollisionPoint(owner.lock().get()->box.GetCenter(),GetEdge());
            if((collisionpoint.x != 0) && (collisionpoint.y != 0)){
                if(cachepoint.GetDistance(collisionpoint.x,collisionpoint.y) > 25){
                    HitEffect(spritefiles["spark2"],5,0.04,0.20,collisionpoint);
                    cachepoint = collisionpoint;
                }
                Component *component1 = owner.lock().get()->GetComponent("Player");
                Player *player = dynamic_cast<Player*>(component1);
                // component1->KeepStill(true,1);
                // KeepStill(true,1);
                Rect collisionrect = Rect(collisionpoint.x,collisionpoint.y,0,0);
                Vec2 ownercenter = owner.lock().get()->box.GetCenter();
                float collisiondist = collisionpoint.GetDistance(ownercenter.x,ownercenter.y);
                float wallknockback = 200 + (300 * (30/(collisiondist)));
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
        Component *colliderComp = other.GetComponent("Collider");
        Vec2 collisionpoint = {0,0};
        Collider *collider = nullptr;
        if(colliderComp){
            collider = dynamic_cast<Collider*>(colliderComp);
            collisionpoint = physics->GetCollisionPoint(GetEdge(false),collider->box.GetCenter(),collider->box,ToPI(other.angleDeg));
        }
        if(hitPlayer){
            Component *component1 = other.GetComponent("Player");
            if(component1){
                KeepStill(true,hitfreezetime);
                hitfreezetime = 0;
            }
            if(hitboxcomponent){
                HitBox *hitbox = dynamic_cast<HitBox*>(hitboxcomponent);
                if(hitbox->HitEnemy()){
                    KeepStill(true,hitfreezetime);
                    hitbox->KeepStill(true,hitfreezetime);
                    hitfreezetime = 0;
                    HitEffect(spritefiles["spark2"],5,0.04,0.20,collisionpoint);
                }
            }
        }
        if(hitEnemy){
            Component *component1 = other.GetComponent("Minion");
            Component *component2 = other.GetComponent("BossHand");
            if(component1){
                Minion *minion = dynamic_cast<Minion*>(component1);
                if((other.box.w != 0) && minion->GetState() != Minion::ATTACKING){
                    KeepStill(true,hitfreezetime);
                    component1->KeepStill(true,hitfreezetime);
                    if(owner.lock()->box.GetCenter().x >= physics->GetCollider()->box.GetCenter().x){ //DIREITA POSITIVO SANGUE1 ESQUERDA NEGATIVO SANGUE1
                        if(associated.angleDeg <= 0){
                            HitEffect(spritefiles["sangue1"],8,0.04,0.32,collisionpoint,true, {35,0});
                        }else{
                            HitEffect(spritefiles["sangue2"],8,0.04,0.32,collisionpoint,true, {35,-50});
                        }
                    }else{
                        if(associated.angleDeg >= 0){
                            HitEffect(spritefiles["sangue1"],8,0.04,0.32,collisionpoint,false, {-35,0}); //DIREITA
                        }else{
                            HitEffect(spritefiles["sangue2"],8,0.04,0.32,collisionpoint,false, {-35,-50}); //DIREITA
                        }
                    }
                }
                hitfreezetime = 0;
                // knockback.x = 0;
                // knockback.y = 0;
            }
            if(component2){
                if((other.box.w != 0)){
                    std::cout << "test" << std::endl;
                    if(owner.lock()->box.GetCenter().x >= physics->GetCollider()->box.GetCenter().x){ //DIREITA POSITIVO SANGUE1 ESQUERDA NEGATIVO SANGUE1
                        if(associated.angleDeg <= 0){
                            HitEffect(spritefiles["sangue1"],8,0.04,0.32,collisionpoint,true, {70,20});
                        }else{
                            HitEffect(spritefiles["sangue2"],8,0.04,0.32,collisionpoint,true, {70,-50});
                        }
                    }else{
                        if(associated.angleDeg >= 0){
                            HitEffect(spritefiles["sangue1"],8,0.04,0.32,collisionpoint,false, {-70,20}); //DIREITA
                        }else{
                            HitEffect(spritefiles["sangue2"],8,0.04,0.32,collisionpoint,false, {-70,-50}); //DIREITA
                        }
                    }
                }
                hitfreezetime = 0;
            }
            if(hitboxcomponent){
                HitBox *hitbox = dynamic_cast<HitBox*>(hitboxcomponent);
                if(hitbox->HitPlayer()){
                    KeepStill(true,hitfreezetime);
                    hitbox->KeepStill(true,hitfreezetime);
                    hitfreezetime = 0;
                    if(collider){
                         HitEffect(spritefiles["spark2"],5,0.04,0.20,collider->box.GetCenter());
                    }
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

void HitBox::KeepStill(bool freeze,float time){
    Component::KeepStill(freeze,time);
    if(component){
        component->KeepStill(freeze,time);
    }
}

void HitBox::HitEffect(std::string file,int frames,float frametime,float duration,Vec2 point,bool flip,Vec2 offset){
    GameObject *effectObj = new GameObject();
    Sprite *effect = new Sprite(*effectObj,file,frames,frametime,duration,false);
    if(flip){
        effect->Flip();
    }
    effectObj->box.x = point.x - effectObj->box.w/2 + offset.x;
    effectObj->box.y = point.y - effectObj->box.h/2 + offset.y;
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

Vec2 HitBox::GetEdge(bool outer){
    float angle = ToPI(associated.angleDeg);
    Collider *collider = physics->GetCollider();
    Vec2 center = physics->GetCollider()->box.GetCenter();
    Vec2 point1;
    Vec2 point2;
    Vec2 outerEdge = {0,0};
    Vec2 innerdge = {0,0};
    point1 = (Vec2(collider->box.x, collider->box.y + collider->box.h/2) - center).GetRotated( angle ) + center;
    point2 = (Vec2(collider->box.x + collider->box.w,collider-> box.y + collider->box.h/2) - center).GetRotated( angle ) + center;
    if(owner.lock()->box.GetCenter().x >= center.x){
        outerEdge = point1;
        innerdge = point2;
    }else{
        outerEdge = point2;
        innerdge = point1;
    }
    if(outer){
        return outerEdge;
    }else{
        return innerdge;
    }
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

