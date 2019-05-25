#include "../include/Boss.h"
#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Player.h"
#include "../include/HitBox.h"
#include "../include/Camera.h"
#include "../include/WindowEffects.h"
#include "../include/DeadBody.h"
#include "../include/GameData.h"

Boss::Boss(GameObject& associated) : Component(associated){
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    gravspeed = 2000;

    hp = 60;
    attackrange = 150;
    sightrange = 500;
    damageCooldown = 0;
    invincibilitytimer =  new Timer();
    damagetimer = new Timer();
    attackdelay = new Timer();

    state = IDLE;

    this->attacktimer = new Timer();
    sightangle = 0;
    spritefiles = GameData::GetSpritesFiles("assets/img/info/player.txt");
    this->bosssprite =  new Sprite(associated,spritefiles["transparent"],32,0.08);
    associated.AddComponent(bosssprite);


    minionspawntimer = new Timer();
    float posX = associated.box.x;
    float posY = associated.box.y;
    for(int i = 0; i < 5; i++){
        posY += 75;
        for(int j = 0; j < 5; j++){
            SpawnEye({posX,posY});
            posX += 100;
        }
        posX = associated.box.x;
    }
}

Boss::~Boss(){
    bosssprite = nullptr;
    delete invincibilitytimer;
    delete damagetimer;
    delete attacktimer;
    delete attackdelay;
    delete minionspawntimer;
}

void Boss::Start(){
}

void Boss::Update(float dt){
    switch(state){
        case IDLE:
            IdleState(dt);
            break;
        case CHASING:
            ChasingState(dt);
            break;
        case ATTACKING:
            AttackState(dt);
            break;
        default:
            break;
    }

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
        if(!associated.IsDead()){
            KillBoss();
        }
    }
}

void Boss::DamageBoss(int damage){
    hp -= damage;
}

void Boss::KillBoss(){
}

void Boss::SpawnEye(Vec2 pos){
    GameObject *eyeObj =  new GameObject();
    Circle bounds = Circle(pos.x,pos.y,35);
    Eye *eye = new Eye(*eyeObj,bounds,30);
    eye->SetParallax(0.5);
    eyeObj->AddComponent(eye);
    std::weak_ptr<GameObject> eyeweak = Game::GetInstance().GetCurrentState().AddObject(eyeObj);
    eyes.push_back(eyeweak);
}

void Boss::AttackState(float dt){
}

void Boss::ChasingState(float dt){
}

void Boss::IdleState(float dt){
    minionspawntimer->Update(dt);
    if(minionspawntimer->Get() > 2){
        int minionspawn = (rand() % 5) + 1;
        minionspawn = 5;
        if(minionspawn == 5){
            SpawnMinion();
            std::cout << "SPAWNED MINION" << dt << std::endl;
        }
        minionspawntimer->Restart();
    }
}

void Boss::SpawnMinion(){
    int eyen = (rand() % eyes.size());
    Component *eyecomp = eyes[eyen].lock()->GetComponent("Eye");
    Eye *eye =  dynamic_cast<Eye*>(eyecomp);
    eye->SpawnMinion();
}

void Boss::IdleHandle(float dt){
}

void Boss::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    bosssprite->SetFrameCount(framecount);
    bosssprite->SetFrameTime(frametime);
    bosssprite->SetRepeat(repeat);
    bosssprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
}

void Boss::Render(){
    #ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS) && Player::player){
        WindowEffects::DrawBox(sightline,sightangle,255,0,0);
	}
    #endif // DEBUG
}

bool Boss::Is(std::string type){
    if(type == "Boss"){
        return true;
    }else{
        return false;
    }
}

void Boss::NotifyCollision(GameObject& other){
    if(!invincibilitytimer->Started()){
        
    }
}
