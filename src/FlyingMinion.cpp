#include "../include/FlyingMinion.h"
#include "../include/Collider.h"
#include "../include/Player.h"
#include "../include/HitBox.h"
#include "../include/Camera.h"
#include "../include/WindowEffects.h"
#include "../include/DeadBody.h"
#include "../include/GameData.h"
#include "../include/Spit.h"

FlyingMinion::FlyingMinion(GameObject& associated,minionState startingState) : Component(associated){
    //Movement related variables
    speed.x = 0;
    speed.y = 0;
    gravspeed = 0;

    maxspeed = 300;
    aspeed = 200;
    despeed = 2000;

    //Minion health
    hp = 60;


    //Sight related variables
    sightrange = 800;
    sightanglerange[0] = PI/2 - PI/8;                 //Range of 45 deg, centered right bellow minion (90 degrees)
    sightanglerange[1] = PI/2 + PI/8;

    downsightrange = 1250;
    downsightanglerange[0] = PI/2 - PI/12;             //Range of 30 deg, centered right bellow minion (90 degrees)
    downsightanglerange[1] = PI/2 + PI/12;

    sightangle = 0;
    downsightangle = 0;

    minyspit = 500;
    maxyspit = 750;


    //Time related variables
    damageCooldown = 0;

    invincibilitytimer =  new Timer();
    damagetimer = new Timer();
    spitdelay = new Timer();
    idletimer = new Timer();
    spittimer = new Timer();


    //State related variables
    idle = false;
    spitted = false;
    state = startingState;


    //Add components to the Game Object
    std::vector<std::string> spritefile;
    spritefile.push_back("assets/img/info/flyingminion.txt");
    spritefiles = GameData::GetSpritesFiles(spritefile);
    Sprite *minion =  new Sprite(associated,spritefiles["idle"],FLYINGMINION_IDLE_FC,FLYINGMINION_IDLE_FT);
    minionsprite = minion;
    associated.AddComponent(minion);

    physics = new Physics(associated,&speed,false,true);
    associated.AddComponent(physics);
    physics->SetCollider(0.5,0.65,0,0);

}

FlyingMinion::~FlyingMinion(){
    minionsprite = nullptr;
    physics = nullptr;
    delete idletimer;
    delete invincibilitytimer;
    delete damagetimer;
    delete spitdelay;
}

void FlyingMinion::Start(){

}

void FlyingMinion::Update(float dt){
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


    DefineState(distanceToPlayer);
        

    }
    switch(state){
        case IDLE:
            IdleState(dt);
            break;
        case POSITIONING:
            PositioningState(dt);
            break;
        case SPITTING:
            // std::cout<<"SPITTING"<<std::endl;
            SpittingState(dt);
            break;
        default:
            break;
    }

    XMovement(dt);
    YMovement(dt);

    if(spitdelay->Started()){
        spitdelay->Update(dt);
        if(spitdelay->Get() > FLYINGMINION_SPITDELAY){
            spitdelay->Restart();
        }
    }
    if(damagetimer->Started()){
        damagetimer->Update(dt);
        if(damagetimer->Get() > 0.20){
            damagetimer->Restart();
            if(state == IDLE){
                SetSprite(spritefiles["idle"],FLYINGMINION_IDLE_FC,FLYINGMINION_IDLE_FT);
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
                KillFlyingMinion();
            }
        }
    }
}

void FlyingMinion::DamageFlyingMinion(int damage){
    hp -= damage;
    if(!damagetimer->Started() && (hp > 0)){
        SetSprite(spritefiles["damage"],FLYINGMINION_DAMAGE_FC,FLYINGMINION_DAMAGE_FT, false);
        damagetimer->Delay(0);
    }else if(hp <= 0){
        KillFlyingMinion();
    }
}

void FlyingMinion::KillFlyingMinion(){
    GameObject *deadObj = new GameObject();
    int animrand = rand() % 2 + 1;
    if(animrand == 1){
        Sprite *deadsprite = new Sprite(*deadObj,spritefiles["deadbehind"],FLYINGMINION_DEADBEHIND_FC,FLYINGMINION_DEADBEHIND_FT,0,false,true);
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
        Sprite *deadsprite = new Sprite(*deadObj,spritefiles["deadfront"],FLYINGMINION_DEADFRONT_FC,FLYINGMINION_DEADFRONT_FT,0,false,true);
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


void FlyingMinion::XMovement(float dt){

    physics->PerformXMovement(dt);//Perfoms Movement if Allowed
}

void FlyingMinion::YMovement(float dt){
    physics->PerformYMovement(dt);//Performs movement if it is allowed
    physics->PerformGravity(gravspeed,dt); // Gravity
}

void FlyingMinion::CreateSpit(){
    GameObject *spitObj = new GameObject();
    Vec2 player = Player::player->GetPosition();
    spitObj->angleDeg = GetPosition().GetAngle(player.x,player.y - 100) * 360 / (2 * 3.14);

    Spit *spit = new Spit(*spitObj, associated, 500);
    spitObj->AddComponent(spit);
    spitObj->box.SetCenter(associated.box.GetCenter());
    Game::GetInstance().GetCurrentState().AddObject(spitObj);
}

void FlyingMinion::IdleState(float dt){
    IdleHandle(dt);
    physics->PerformXDeceleration(despeed,dt);
    physics->PerformYDeceleration(despeed,dt);
}

void FlyingMinion::PositioningState(float dt){
     Vec2 player = Player::player->GetPosition();

    //Perform needed aceleration
    if(dirToPlayer.x != 0){
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
    
    if(dirToPlayer.y != 0){
        if(dirToPlayer.y == 1){
            physics->PerformYAcceleration(false,aspeed,dt);
        }
        else{
            physics->PerformYAcceleration(true,aspeed,dt);
        }
    }
}

void FlyingMinion::SpittingState(float dt){
    
    physics->PerformYDeceleration(despeed,dt);
    physics->PerformXDeceleration(despeed,dt);

    spittimer->Update(dt);
    if((spittimer->Get() > ((FLYINGMINION_SPITTING_FC-3) * FLYINGMINION_SPITTING_FT) )){
        CreateSpit();
        spitted = true;
        spittimer->Restart();
    }

    if((spittimer->Get() > 3 * FLYINGMINION_SPITTING_FT) && spitted){
        spitdelay->Update(dt);
        spittimer->Restart();
        spitted = false;
    }

}

void FlyingMinion::DefineState(float distanceToPlayer){
    minionState newState;
    Vec2 player = Player::player->GetPosition();
    Vec2 positioning = PositioningHandle();


    bool inRange, inPosition, canSpit;

    inRange = (distanceToPlayer != sightrange) || CheckPlayerDown();        //True if it can see the player
    inPosition = positioning == Vec2(0,0);                                  //True if flying minion is in the position to spit
    canSpit = !spitdelay->Started();                                        //True if minion hasn't spitted yet (delay hasn't been set yet)


    //Select state
    if(!inRange || (inPosition && !canSpit)){
        newState = IDLE;
    }
    else if(inRange && !inPosition){
        newState = POSITIONING;
    }
    else{
        newState = SPITTING;
    }
    
    //Change sprite, if state has changed
    if(newState != state){
        if(newState == IDLE){
            SetSprite(spritefiles["idle"],FLYINGMINION_IDLE_FC,FLYINGMINION_IDLE_FT);
            spittimer->Restart();
        }
        else if(newState == POSITIONING){
            SetSprite(spritefiles["flying"],FLYINGMINION_FLYING_FC,FLYINGMINION_FLYING_FT);
            dirToPlayer = positioning;
            spittimer->Restart();
        }
        else{
            SetSprite(spritefiles["spitting"],FLYINGMINION_SPITTING_FC,FLYINGMINION_SPITTING_FT);
        }
    }

    state = newState;
}

Vec2 FlyingMinion::PositioningHandle(){
    Vec2 player = Player::player->GetPosition();
    Vec2 minionpos = GetPosition();

    float newx=0, newy=0;

    if(player.y - minionpos.y > maxyspit){
        newy = -1;                          //Fly up
    }
    else{
        if(player.y - minionpos.y < minyspit){
            newy = 1;                       //Fly down
        }
    }
    if(sightangle > sightanglerange[1]){
        newx = -1;                          //Fly to the left
    }
    else{
        if(sightangle < sightanglerange[0]){
            newx = 1;                       //Fly to the right
        }
    }
    return Vec2(newx,newy);
}

void FlyingMinion::IdleHandle(float dt){
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


bool FlyingMinion::CheckPlayerDown(){

    Vec2 minionpos = GetPosition();
    Vec2 player = Player::player->GetPosition();

    int distanceToPlayer = physics->SightTo(minionpos,player.Added(0,-100),downsightrange);

    if(distanceToPlayer < downsightrange){
        downsightangle = minionpos.GetAngle(player.x,player.y - 100);

        if((downsightangle < downsightanglerange[1]) && (downsightangle > downsightanglerange[0])){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

void FlyingMinion::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    minionsprite->SetFrameCount(framecount);
    minionsprite->SetFrameTime(frametime);
    minionsprite->SetRepeat(repeat);
    minionsprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
    physics->GetCollider()->UpdateScale();
}

void FlyingMinion::Render(){
    #ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS) && Player::player){
        WindowEffects::DrawBox(sightline,sightangle,255,0,0);
	}
    #endif // DEBUG
}

bool FlyingMinion::Is(std::string type){
    if(type == "FlyingMinion"){
        return true;
    }else{
        return false;
    }
}

void FlyingMinion::NotifyCollision(GameObject& other){
    if(!invincibilitytimer->Started()){
        Component *component1 = other.GetComponent("HitBox");
        Component *component2 = other.GetComponent("FlyingMinion");
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
                DamageFlyingMinion(hitbox->GetDamage());
                damageCooldown = hitbox->GetDamageCooldown();
                invincibilitytimer->Delay(0);
            }
        }
        if(component2){
            physics->KnockBack(other.box,{5,0},true,{600,400});
        }
    }
}

Vec2 FlyingMinion::GetPosition(){

    return physics->GetCollider()->box.GetCenter();
}

FlyingMinion::minionState FlyingMinion::GetState(){

    return state;
}
