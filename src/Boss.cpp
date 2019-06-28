#include "../include/Boss.h"
#include "../include/Minion.h"
#include "../include/Collider.h"
#include "../include/Player.h"
#include "../include/HitBox.h"
#include "../include/Camera.h"
#include "../include/WindowEffects.h"
#include "../include/DeadBody.h"
#include "../include/GameData.h"
#include "../include/StageState.h"
#include "../include/ParallaxFollower.h"
#include "../include/InputManager.h"

Boss::Boss(GameObject& associated) : Component(associated){
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    gravspeed = 2000;

    hp = 800;
    attackrange = 150;
    sightrange = 500;
    damageCooldown = 0;
    invincibilitytimer =  new Timer();
    idlehandtimer = new Timer();
    returnhandtimer = new Timer();
    damagetimer = new Timer();
    attackdelay = new Timer();

    state = IDLE;

    this->attacktimer = new Timer();
    this->handuptimer = new Timer();
    sightangle = 0;
    spritefiles = GameData::GetSpritesFiles("assets/img/info/boss.txt");
    this->bosssprite =  new Sprite(associated,spritefiles["idle"],24,0.04);
    associated.AddComponent(bosssprite);

    ParallaxFollower *parallaxfollower = new ParallaxFollower(associated,0.5);
    associated.AddComponent(parallaxfollower);

    minionspawntimer = new Timer();

    SpawnHead({associated.box.x + 300,associated.box.y + 45}); //520 80 650 150

    // GameObject *handobj = new GameObject();
    // handobj->box.Transform(associated.box.x + 200,associated.box.y + 750);
    // Sprite *sprite = new Sprite(*handobj,"assets/img/bossIdlemaorascunho.png");
    // ParallaxFollower *parallaxfollower2 = new ParallaxFollower(*handobj,0.5);
    // handobj->AddComponent(parallaxfollower2);
    // handobj->AddComponent(sprite);
    // Game::GetInstance().GetCurrentState().AddObject(handobj);
}

Boss::~Boss(){
    bosssprite = nullptr;
    delete invincibilitytimer;
    delete damagetimer;
    delete attacktimer;
    delete attackdelay;
    delete minionspawntimer;
    delete idlehandtimer;
    delete returnhandtimer;
    delete handuptimer;

    if(!head.expired()){
        head.lock()->RequestDelete();
    }
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

void Boss::HandHitbox(GameObject& hitbox,GameObject& owner,float dt){
    Physics* physics = hitbox.GetPhysics();
    Component *component = hitbox.GetComponent("HitBox");
    physics->PerformGravity(40000,dt);
    physics->PerformYMovement(dt);

    if(physics->IsGrounded()){
        hitbox.RequestDelete();
    }
}

void Boss::InstantiateHitBox(Rect hitbox,float duration,Vec2 knockback){
    GameObject *hitboxObj = new GameObject();
    std::weak_ptr<GameObject> owner = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    HitBox *hitboxcomp = new HitBox(*hitboxObj,hitbox,owner,0,20,duration,duration,false,true,false,knockback,this);
    hitboxcomp->SetFunction(Boss::HandHitbox);
    hitboxObj->AddComponent(hitboxcomp);
    Game::GetInstance().GetCurrentState().AddObject(hitboxObj);
}

void Boss::DamageBoss(int damage){
    hp -= damage;
}

void Boss::KillBoss(){
    for(int i = 0;i < eyes.size();i ++){
        eyes[i].lock()->RequestDelete();
    }
    associated.RequestDelete();
}


void Boss::MoveHead(Vec2 speed,float dt){
    head.lock()->box.x += speed.x * dt;
    head.lock()->box.y += speed.y * dt;

    for(int i = 0;i < eyes.size();i++){
        eyes[i].lock()->box.x += speed.x * dt;
        eyes[i].lock()->box.y += speed.y * dt;
    }
}

void Boss::DestroyHead(){
    head.lock()->RequestDelete();
    for(int i = 0;i < eyes.size();i++){
        eyes[i].lock()->RequestDelete();
    }
}

void Boss::SpawnHead(Vec2 pos){
    GameObject *headobj = new GameObject();
    headobj->box.x = pos.x;
    headobj->box.y = pos.y;
    Sprite* headsprite = new Sprite(*headobj,spritefiles["head"],16,0.04);
    ParallaxFollower *parallaxfollower = new ParallaxFollower(*headobj,0.5);
    headobj->AddComponent(parallaxfollower);
    headobj->AddComponent(headsprite);
    head = Game::GetInstance().GetCurrentState().AddObject(headobj);

    //Up Eyes
    SpawnEye({pos.x + 314,pos.y + 285},{1532,1012});
    SpawnEye({pos.x + 408,pos.y + 285},{1532,1012});
    SpawnEye({pos.x + 508,pos.y + 285},{1532,1012});

    //Down Eyes
    SpawnEye({pos.x + 255,pos.y + 361},{1532,1012});
    SpawnEye({pos.x + 360,pos.y + 361},{1532,1012});
    SpawnEye({pos.x + 463,pos.y + 361},{1532,1012});
    SpawnEye({pos.x + 568,pos.y + 361},{1532,1012});
}

void Boss::SpawnEye(Vec2 pos,Vec2 endpos){
    GameObject *eyeObj =  new GameObject();
    Circle bounds = Circle(pos.x,pos.y,35);
    Eye *eye = new Eye(*eyeObj,bounds,endpos,30);
    ParallaxFollower *parallaxfollower = new ParallaxFollower(*eyeObj,0.5);
    eyeObj->AddComponent(parallaxfollower);
    eyeObj->AddComponent(eye);
    int placeofplayer = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("Player");
    std::weak_ptr<GameObject> eyeweak = Game::GetInstance().GetCurrentState().AddObject(eyeObj,placeofplayer);
    eyes.push_back(eyeweak);
}

void Boss::CatchParallax(){
    for(int i = 0;i < eyes.size();i++){
        Component *comp = eyes[i].lock()->GetComponent("ParallaxFollower");
        if(comp){
            ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
            parallaxfollower->SetParallax(0.5,true);
        }
    }

    GameObject *headObj = head.lock().get();
    Component *comp2 = headObj->GetComponent("ParallaxFollower");
    if(comp2){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp2);
        parallaxfollower->SetParallax(0.5,true);
    }

    Component *comp3 = associated.GetComponent("ParallaxFollower");
    if(comp3){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp3);
        parallaxfollower->SetParallax(0.5,true);
    }
}

void Boss::StopParallax(){

    for(int i = 0;i < eyes.size();i++){
        Component *comp = eyes[i].lock()->GetComponent("ParallaxFollower");
        if(comp){
            ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
            parallaxfollower->SetParallax(1);
        }
    }

    GameObject *headObj = head.lock().get();
    Component *comp = headObj->GetComponent("ParallaxFollower");
    if(comp){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
        parallaxfollower->SetParallax(1);
    }

    Component *comp2 = associated.GetComponent("ParallaxFollower");
    if(comp2){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp2);
        parallaxfollower->SetParallax(1);
    }
}

void Boss::AttackState(float dt){
    if(!attacktimer->Started() && !idlehandtimer->Started() && !returnhandtimer->Started() && !handuptimer->Started()){
        StopParallax();
        Rect LeftHandArea = Rect(associated.box.x + 0,associated.box.y + 800,200,100);
        // WindowEffects::AddBoxToDraw(LeftHandArea,0);
        if(LeftHandArea.Contains(Player::player->GetPosition())){
            SetSprite(spritefiles["uplefthand"],29,0.04,false,{15,-120});
            handuptimer->Delay(dt);
            screenshake = false;
        }else{
            state = IDLE;
            handuptimer->Restart();
        }
    }
    if(handuptimer->Started()){
        handuptimer->Update(dt);
        if(handuptimer->Get() >= 1.16){
            SetSprite(spritefiles["attacklefthand"],22,0.04,true);
            handuptimer->Restart();
            attacktimer->Delay(dt);
            InstantiateHitBox({associated.box.x + 200,associated.box.y + 500,300,100},2,{1000,300});
        }
    }
    if(attacktimer->Started()){
        attacktimer->Update(dt);
        if((attacktimer->Get() >= 0.08) && (!screenshake)){
            screenshake = true;
            Camera::ShakeScreen(1,60);
        }
        if(attacktimer->Get() >= 0.88){
            SetSprite(spritefiles["idlelefthand"],20,0.04,true,{30,-70});
            idlehandtimer->Delay(dt);
            attacktimer->Restart();
        }
    }
    if(idlehandtimer->Started()){
        idlehandtimer->Update(dt);
        if(idlehandtimer->Get() >= 3){
            SetSprite(spritefiles["returnlefthand"],6,0.04,false);
            returnhandtimer->Delay(dt);
            idlehandtimer->Restart();
        }
    }
    if(returnhandtimer->Started()){
        returnhandtimer->Update(dt);
        if(returnhandtimer->Get() >= 0.24){
            returnhandtimer->Restart();
            state = IDLE;
            SetSprite(spritefiles["idle"],24,0.04,true,{-45,190});
            CatchParallax();
        }
    }
}

void Boss::ChasingState(float dt){
}

void Boss::IdleState(float dt){
    InputManager *input =  &(InputManager::GetInstance());

    if(StageState::MapCollisionLoaded()){
        if(bosssprite->GetCurrentFrame() >= 12){
            MoveHead({0,55},dt);
        }else{
            MoveHead({0,-55},dt);
        }
    }
    Rect LeftHandArea = Rect(associated.box.x + 0,associated.box.y + 800,200,100);
    // WindowEffects::AddBoxToDraw(LeftHandArea,0);
    if(LeftHandArea.Contains(Player::player->GetPosition())){
        this->state = ATTACKING;
        StopParallax();
    }

    if(input->KeyPress(SDLK_8)){
        Vec2 mousepos = Vec2(input->GetMouseX()*2,input->GetMouseY()*2).Added(Camera::pos.x,Camera::pos.y);
        std::cout << mousepos.x - associated.box.x << " " << mousepos.y - associated.box.y << std::endl;
        // SetSprite(spritefiles["attacklefthand"],51,0.04,false,{25,-190});
    }
    // if(input->KeyPress(SDLK_7)){
    //     StopParallax();
    // }
    // if(input->KeyPress(SDLK_8)){
    //     CatchParallax();
    // }

    // if(Game::GetInstance().GetCurrentState().GetNumberOf("Minion") < 5){
    //     minionspawntimer->Update(dt);
    //     if(minionspawntimer->Get() > 1){
    //         int minionspawn = (rand() % 5) + 1;
    //         // minionspawn = 5;
    //         if(minionspawn == 5){
    //             SpawnMinion();
    //             // InstantiateHitBox({Player::player->GetPosition().Added(-300,-600),300,100},2,{400,200});
    //             // std::cout << "SPAWNED MINION" << dt << std::endl;
    //         }
    //         minionspawntimer->Restart();
    //     }
    // }
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

    Rect lifebar =  Rect((Camera::window.x - hp*2)/2,Camera::window.y - 80,hp*2,50);

    WindowEffects::FillRect(lifebar,0,0,0,255);
    WindowEffects::FillRect(lifebar.Added(6,6,-12,-12),255,0,0,255);
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
        Component *component1 = other.GetComponent("HitBox");
        if(component1){
            HitBox *hitbox = dynamic_cast<HitBox*>(component1);
            if((hitbox)  && hitbox->HitEnemy()){
                DamageBoss(hitbox->GetDamage());
                damageCooldown = hitbox->GetDamageCooldown();
                invincibilitytimer->Delay(0);
            }
        }
    }
}
