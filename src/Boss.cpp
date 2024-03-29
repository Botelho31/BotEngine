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
#include "../include/BossHand.h"
#include "../include/Sound.h"

#define BOSSPARALLAX 1.2
#define INITIALPOSX 70
#define INITIALPOSY 555
#define HEADOFFSETX 250
#define HEADOFFSETY 45


Boss *Boss::boss;

Boss::Boss(GameObject& associated) : Component(associated){
    rampage1 = false;
    rampage2 = false;
    rampage3 = false;
    boss = this;
    bossrisehalf = false;
    bossrisetimer = new Timer();
    bossdeathtimer = new Timer();
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    gravspeed = 2000;

    hp = BOSSHP;
    attackrange = 150;
    sightrange = 500;
    damageCooldown = 0;
    invincibilitytimer =  new Timer();
    idlehandtimer = new Timer();
    returnhandtimer = new Timer();
    damagetimer = new Timer();
    attackdelay = new Timer();
    appearingtimer = new Timer();

    state = APPEARING;

    this->attacktimer = new Timer();
    this->handuptimer = new Timer();
    sightangle = 0;
    std::vector<std::string> spritefile;
    spritefile.push_back("assets/img/info/boss.txt");
    spritefile.push_back("assets/img/info/effects.txt");
    this->spritefiles = GameData::GetSpritesFiles(spritefile);
    this->bosssprite =  new Sprite(associated,spritefiles["rise1"],23,0.08);
    associated.AddComponent(bosssprite);

    ParallaxFollower *parallaxfollower = new ParallaxFollower(associated,BOSSPARALLAX);
    associated.AddComponent(parallaxfollower);

    minionspawntimer = new Timer();

    // SpawnHead({associated.box.x + 300,associated.box.y + 45}); //520 80 650 150
}

Boss::~Boss(){
    boss = nullptr;
    bosssprite = nullptr;
    delete bossrisetimer;
    delete appearingtimer;
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
    if(!hand.expired()){
        DestroyHand();
    }
    Game::GetInstance().GetMusic()->Stop();
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
        case HANDATTACKING:
            HandAttackState(dt);
            break;
        case RAMPAGEATTACKING:
            RampageAttackState(dt);
            break;
        case APPEARING:
            AppearingState(dt);
            break;
        case BITING:
            BiteState(dt);
            break;
        case DYING:
            if(!bossdeathtimer->Started()){
                DestroyHead();
                SetSprite(spritefiles["deathstand"],1,0,false,{0,-125});
                bossdeathtimer->Delay(0);
                screenshake = false;
            }else{
                bossdeathtimer->Update(dt);
                if((bossdeathtimer->Get() >= 2) && (!screenshake)){
                    SetSprite(spritefiles["death"],15,0.12,false);
                    screenshake = true;
                }
                if(bossdeathtimer->Get() >= 3.8){
                    Camera::ShakeScreen(4,100);
                    associated.RequestDelete();
                }
            }
            break;
        default:
            break;
    }

    if(attackdelay->Started()){
        attackdelay->Update(dt);
        if(attackdelay->Get() > 5){
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
    
}

void Boss::SpeedUpParallax(){
    if(eyes.size() > 0){
        for(int i = 0;i < eyes.size();i++){
            Component *comp = eyes[i].lock()->GetComponent("ParallaxFollower");
            if(comp){
                ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
                parallaxfollower->Jump();
            }
        }
    }

    if(!head.expired()){
        GameObject *headObj = head.lock().get();
        Component *comp2 = headObj->GetComponent("ParallaxFollower");
        if(comp2){
            ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp2);
            parallaxfollower->Jump();
        }
    }

    Component *comp3 = associated.GetComponent("ParallaxFollower");
    if(comp3){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp3);
        parallaxfollower->Jump();
    }
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
    eyes.clear();
}

void Boss::SpawnHead(Vec2 pos){
    GameObject *headobj = new GameObject();
    headobj->box.x = pos.x;
    headobj->box.y = pos.y;
    Sprite* headsprite = new Sprite(*headobj,spritefiles["head"],16,0.04);
    ParallaxFollower *parallaxfollower = new ParallaxFollower(*headobj,BOSSPARALLAX);
    headobj->AddComponent(parallaxfollower);
    headobj->AddComponent(headsprite);
    parallaxfollower->SetOriginalPos({INITIALPOSX + 300,INITIALPOSY + 45});
    int placeofplayer = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("Player");
    head = Game::GetInstance().GetCurrentState().AddObject(headobj,placeofplayer);

    //Up Eyes
    SpawnEye({pos.x + 314,pos.y + 285},{INITIALPOSX + 314 + HEADOFFSETX,INITIALPOSY + 285 + HEADOFFSETY});
    SpawnEye({pos.x + 408,pos.y + 285},{INITIALPOSX + 408  + HEADOFFSETX,INITIALPOSY + 285 + HEADOFFSETY});
    SpawnEye({pos.x + 508,pos.y + 285},{INITIALPOSX + 508  + HEADOFFSETX,INITIALPOSY + 285 + HEADOFFSETY});

    //Down Eyes
    SpawnEye({pos.x + 255,pos.y + 361},{INITIALPOSX + 255 + HEADOFFSETX,INITIALPOSY + 361 + HEADOFFSETY});
    SpawnEye({pos.x + 360,pos.y + 361},{INITIALPOSX + 360 + HEADOFFSETX,INITIALPOSY + 361 + HEADOFFSETY});
    SpawnEye({pos.x + 463,pos.y + 361},{INITIALPOSX + 463 + HEADOFFSETX,INITIALPOSY + 361 + HEADOFFSETY});
    SpawnEye({pos.x + 568,pos.y + 361},{INITIALPOSX + 568 + HEADOFFSETX,INITIALPOSY + 361 + HEADOFFSETY});
}

void Boss::SpawnEye(Vec2 pos,Vec2 endpos){
    GameObject *eyeObj =  new GameObject();
    Circle bounds = Circle(pos.x,pos.y,35);
    Eye *eye = new Eye(*eyeObj,bounds,endpos,30);
    ParallaxFollower *parallaxfollower = new ParallaxFollower(*eyeObj,BOSSPARALLAX);
    parallaxfollower->SetOriginalPos(endpos);
    eyeObj->AddComponent(parallaxfollower);
    eyeObj->AddComponent(eye);
    int placeofplayer = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("Player");
    std::weak_ptr<GameObject> eyeweak = Game::GetInstance().GetCurrentState().AddObject(eyeObj,placeofplayer);
    eyes.push_back(eyeweak);
}

void Boss::CatchParallax(){
    if(eyes.size() > 0){
        for(int i = 0;i < eyes.size();i++){
            Component *comp = eyes[i].lock()->GetComponent("ParallaxFollower");
            if(comp){
                ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
                parallaxfollower->SetParallax(BOSSPARALLAX,true);
            }
        }
    }

    if(!head.expired()){
        GameObject *headObj = head.lock().get();
        Component *comp2 = headObj->GetComponent("ParallaxFollower");
        if(comp2){
            ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp2);
            parallaxfollower->SetParallax(BOSSPARALLAX,true);
        }
    }

    Component *comp3 = associated.GetComponent("ParallaxFollower");
    if(comp3){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp3);
        parallaxfollower->SetParallax(BOSSPARALLAX,true);
    }
}

void Boss::StopParallax(){
    if(eyes.size() > 0){
        for(int i = 0;i < eyes.size();i++){
            Component *comp = eyes[i].lock()->GetComponent("ParallaxFollower");
            if(comp){
                ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
                parallaxfollower->SetParallax(1);
            }
        }
    }

    if(!head.expired()){
        GameObject *headObj = head.lock().get();
        Component *comp = headObj->GetComponent("ParallaxFollower");
        if(comp){
            ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp);
            parallaxfollower->SetParallax(1);
        }
    }

    Component *comp2 = associated.GetComponent("ParallaxFollower");
    if(comp2){
        ParallaxFollower *parallaxfollower = dynamic_cast<ParallaxFollower*>(comp2);
        parallaxfollower->SetParallax(1);
    }
}

void Boss::AppearingState(float dt){
    if(!appearingtimer->Started() && !bossrisetimer->Started() && !attacktimer->Started() && !handuptimer->Started()){
        SpeedUpParallax();
        StopParallax();
        Game::GetInstance().GetMusic()->Open("assets/audio/musics/bossintro.ogg");
        Game::GetInstance().GetMusic()->Play();
        appearingtimer->Delay(dt);
        bossrisetimer->Delay(dt);
        bossrisehalf = false;
    }
    if(bossrisetimer->Started()){
        bossrisetimer->Update(dt);
        if((bossrisetimer->Get() >= 1.84) && !bossrisehalf){
            SetSprite(spritefiles["rise2"],23,0.08,false);
            bossrisehalf = true;
        }
        if(bossrisetimer->Get() >= 3.68){
            bossrisetimer->Restart();
            if(!GameData::playerSword){
                SetSprite(spritefiles["headup"],19,0.04,false,{0,-134});
                screenshake = false;
                hitboxinstantiated = false;
                handuptimer->Delay(dt);
            }else{
                SetSprite(spritefiles["idle"],24,0.04,true);
                attacktimer->Restart();
                SpawnHead({associated.box.x + 300,associated.box.y + 45});
                StopParallax();
            }
        }
    }
    
    if(handuptimer->Started()){
        handuptimer->Update(dt);
        if(handuptimer->Get() >= 0.76){
            SetSprite(spritefiles["bite"],34,0.04,false);
            handuptimer->Restart();
            attacktimer->Delay(dt);
        }
    }
    if(attacktimer->Started()){
        attacktimer->Update(dt);
        if((attacktimer->Get() >= 0.08) && (!screenshake)){
            screenshake = true;
            InstantiateHitBox(Rect(associated.box.x + 450,associated.box.y + 780,700,120),2,{1000,300});
            Camera::ShakeScreen(1,200);

            if(Player::player){
                Player::player->SetHealth(0);
            }
        }
        if(attacktimer->Get() >= 1.36){
            SetSprite(spritefiles["idle"],24,0.04,true,{0,134});
            attacktimer->Restart();
            SpawnHead({associated.box.x + 300,associated.box.y + 45});
        }
    }

    if(appearingtimer->Started()){
        appearingtimer->Update(dt);
        if(appearingtimer->Get() > 9){
            appearingtimer->Restart();
            if(GameData::playerAlive){
                GameData::currentMusic = "assets/audio/musics/boss.ogg";
                Game::GetInstance().GetMusic()->Open("assets/audio/musics/boss.ogg");
                Game::GetInstance().GetMusic()->Play();
            }   
            state = IDLE;
            CatchParallax();
        }
    }
}

void Boss::RampageAttackState(float dt){
    if(!attacktimer->Started() && !idlehandtimer->Started() && !returnhandtimer->Started() && !handuptimer->Started()){
        StopParallax();
        if(Player::player->GetPosition().x <= associated.box.GetCenter().x){
            SetSprite(spritefiles["uplefthand"],29,0.01,false,{0,-120});
            handuptimer->Delay(dt);
            screenshake = false;
            lefthand = true;
            righthand = false;
        }
        if(Player::player->GetPosition().x > associated.box.GetCenter().x){
            SetSprite(spritefiles["uprighthand"],29,0.01,false,{0,-120});
            handuptimer->Delay(dt);
            screenshake = false;
            lefthand = false;
            righthand = true;
        }
        rampagecount = 0;
    }
    if(handuptimer->Started()){
        handuptimer->Update(dt);
        if(handuptimer->Get() >= 0.29){
            if(lefthand){
                SetSprite(spritefiles["attacklefthand"],22,0.04,true);
                InstantiateHitBox({associated.box.x + 250,associated.box.y + 500,350,100},2,{1000,300});
            }else{
                SetSprite(spritefiles["attackrighthand"],22,0.04,true);
                InstantiateHitBox({associated.box.x + 1300,associated.box.y + 500,350,100},2,{1000,300});
            }
            handuptimer->Restart();
            attacktimer->Delay(dt);
        }
    }
    if(attacktimer->Started()){
        attacktimer->Update(dt);
        if((attacktimer->Get() >= 0.04) && (!screenshake)){
            screenshake = true;
            Camera::ShakeScreen(0.37,60);
            attacktimer->Restart();
            returnhandtimer->Delay(dt);
            if(lefthand){
                SetSprite(spritefiles["returnlefthand"],6,0.01,false);
            }else{
                SetSprite(spritefiles["returnrighthand"],6,0.01,false);
            }
        }
    }
    if(returnhandtimer->Started()){
        returnhandtimer->Update(dt);
        if(returnhandtimer->Get() >= 0.06){
            returnhandtimer->Restart();
            if(rampagecount <= 10){
                if(lefthand){
                    SetSprite(spritefiles["uprighthand"],29,0.01,false);
                    handuptimer->Delay(dt);
                    screenshake = false;
                    lefthand = false;
                    righthand = true;
                    rampagecount++;
                }else{
                    SetSprite(spritefiles["uplefthand"],29,0.01,false);
                    handuptimer->Delay(dt);
                    screenshake = false;
                    lefthand = true;
                    righthand = false;
                    rampagecount ++;
                }
            }else{
                returnhandtimer->Restart();
                state = IDLE;
                SetSprite(spritefiles["idle"],24,0.04,true,{0,120});
                CatchParallax();
                attackdelay->Delay(0);
            }
        }
    }
}

void Boss::HandAttackState(float dt){
    if(!attacktimer->Started() && !idlehandtimer->Started() && !returnhandtimer->Started() && !handuptimer->Started()){
        StopParallax();       
        if(lefthand){
            SetSprite(spritefiles["uplefthand"],29,0.04,false,{0,-120});
            handuptimer->Delay(dt);
            screenshake = false;
            lefthand = true;
            righthand = false;
        }else if(righthand){
            SetSprite(spritefiles["uprighthand"],29,0.04,false,{0,-120});
            handuptimer->Delay(dt);
            screenshake = false;
            lefthand = false;
            righthand = true;
        }else{
            state = IDLE;
            handuptimer->Restart();
            CatchParallax();
        }
    }
    if(handuptimer->Started()){
        handuptimer->Update(dt);
        if(handuptimer->Get() >= 1.16){
            if(lefthand){
                SetSprite(spritefiles["attacklefthand"],22,0.04,true);
                InstantiateHitBox({associated.box.x + 250,associated.box.y + 500,350,100},2,{1000,300});
            }else{
                SetSprite(spritefiles["attackrighthand"],22,0.04,true);
                InstantiateHitBox({associated.box.x + 1300,associated.box.y + 500,350,100},2,{1000,300});
            }
            handuptimer->Restart();
            attacktimer->Delay(dt);
        }
    }
    if(attacktimer->Started()){
        attacktimer->Update(dt);
        if((attacktimer->Get() >= 0.06) && (!screenshake)){
            screenshake = true;
            Camera::ShakeScreen(1,60);
        }
        if((attacktimer->Get() >= 0.44) && (hand.expired())){
            if(lefthand){
                SpawnHand({associated.box.x + 150,associated.box.y + 1120});
            }else{
                SpawnHand({associated.box.x + 1250,associated.box.y + 1120});
            }
        }
        if(attacktimer->Get() >= 0.88){
            if(lefthand){
                SetSprite(spritefiles["idlelefthand"],20,0.04,true);
            }else{
                SetSprite(spritefiles["idlerighthand"],20,0.04,true);
            }
            idlehandtimer->Delay(dt);
            attacktimer->Restart();
        }
    }
    if(idlehandtimer->Started()){
        idlehandtimer->Update(dt);
        if(idlehandtimer->Get() >= 3){
            if(lefthand){
                SetSprite(spritefiles["returnlefthand"],6,0.04,false);
            }else{
                SetSprite(spritefiles["returnrighthand"],6,0.04,false);
            }
            returnhandtimer->Delay(dt);
            idlehandtimer->Restart();
            DestroyHand();
        }
    }
    if(returnhandtimer->Started()){
        returnhandtimer->Update(dt);
        if(returnhandtimer->Get() >= 0.24){
            returnhandtimer->Restart();
            state = IDLE;
            SetSprite(spritefiles["idle"],24,0.04,true,{0,120});
            CatchParallax();
            attackdelay->Delay(0);
        }
    }
}

void Boss::ChasingState(float dt){
}

void Boss::BiteState(float dt){
    if(!attacktimer->Started() && !handuptimer->Started()){
        SetSprite(spritefiles["headup"],19,0.04,false,{0,-134});
        screenshake = false;
        hitboxinstantiated = false;
        handuptimer->Delay(dt);
        DestroyHead();
    }

    if(handuptimer->Started()){
        handuptimer->Update(dt);
        if(handuptimer->Get() >= 0.76){
            SetSprite(spritefiles["bite"],34,0.04,false);
            handuptimer->Restart();
            attacktimer->Delay(dt);
        }
    }
    if(attacktimer->Started()){
        attacktimer->Update(dt);
        if((attacktimer->Get() >= 0.08) && (!screenshake)){
            screenshake = true;
            InstantiateHitBox(Rect(associated.box.x + 450,associated.box.y + 780,700,120),2,{1000,300});
            Camera::ShakeScreen(1,200);
        }
        if((attacktimer->Get() >= 0.24) && (headhitbox.expired())){
            SpawnHeadHitbox({associated.box.x + 600,associated.box.y + 1000});
        }
        if(attacktimer->Get() >= 1.36){
            state = IDLE;
            SetSprite(spritefiles["idle"],24,0.04,true,{0,134});
            attacktimer->Restart();
            SpawnHead({associated.box.x + 300,associated.box.y + 45});
            CatchParallax();
            DestroyHeadHitbox();
            attackdelay->Delay(0);
        }
    }
}

void Boss::SpawnHeadHitbox(Vec2 pos){
    GameObject *bosshandObj =  new GameObject();
    bosshandObj->box.w = 600;
    bosshandObj->box.h = 650;
    std::weak_ptr<GameObject> bossweakptr =  Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    BossHand *bosshand = new BossHand(*bosshandObj,bossweakptr,pos);
    bosshandObj->AddComponent(bosshand);
    std::weak_ptr<GameObject> handweakptr =  Game::GetInstance().GetCurrentState().AddObject(bosshandObj);
    headhitbox = handweakptr;
}

void Boss::DestroyHeadHitbox(){
    if(!headhitbox.expired()){
        headhitbox.lock()->RequestDelete();
    }
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
    Rect LeftHandArea = Rect(associated.box.x + 0,associated.box.y + 780,300,120);
    Rect RightHandArea = Rect(associated.box.x + 1130,associated.box.y + 780,300,120);
    Rect BiteArea = Rect(associated.box.x + 450,associated.box.y + 780,500,120);
    if(!attackdelay->Started()){
        if(LeftHandArea.Contains(Player::player->GetPosition())){
            this->state = HANDATTACKING;
            StopParallax();
            lefthand = true;
            righthand = false;
        }
        if(RightHandArea.Contains(Player::player->GetPosition())){
            this->state = HANDATTACKING;
            StopParallax();
            righthand = true;
            lefthand = false;
        }
        if(BiteArea.Contains(Player::player->GetPosition())){
            this->state = BITING;
            StopParallax();
        }
        if(hp <= (BOSSHP - (BOSSHP/3))){
            if(!rampage1){
                rampage1 = true;
                state = RAMPAGEATTACKING;
                StopParallax();
            }
        }
        if(hp <= (BOSSHP - (BOSSHP/3)*2)){
            if(!rampage2){
                rampage2 = true;
                state = RAMPAGEATTACKING;
                StopParallax();
            }
        }
        if(hp <= (BOSSHP - ((BOSSHP/3)*3) + 100)){
            if(!rampage3){
                rampage3 = true;
                state = RAMPAGEATTACKING;
                StopParallax();
            }
        }
    }

    if(hp <= 0){
        state = DYING;
    }

    if(Game::GetInstance().GetCurrentState().GetNumberOf("Minion") < 5){
        minionspawntimer->Update(dt);
        if(minionspawntimer->Get() > 1){
            int minionspawn = (rand() % 5) + 1;
            // minionspawn = 5;
            if(minionspawn == 5){
                SpawnMinion();
                // InstantiateHitBox({Player::player->GetPosition().Added(-300,-600),300,100},2,{400,200});
                // std::cout << "SPAWNED MINION" << dt << std::endl;
            }
            minionspawntimer->Restart();
        }
    }
}

void Boss::PlaySoundEffect(std::string file,int times){
    GameObject *effectObj = new GameObject();
    Sound *sound = new Sound(*effectObj,file);
    sound->Play(times,true);
    effectObj->AddComponent(sound);
    Game::GetInstance().GetCurrentState().AddObject(effectObj);
}

void Boss::SpawnHand(Vec2 pos){
    GameObject *bosshandObj =  new GameObject();
    bosshandObj->box.w = 400;
    bosshandObj->box.h = 300;
    std::weak_ptr<GameObject> bossweakptr =  Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    BossHand *bosshand = new BossHand(*bosshandObj,bossweakptr,pos);
    bosshandObj->AddComponent(bosshand);
    std::weak_ptr<GameObject> handweakptr =  Game::GetInstance().GetCurrentState().AddObject(bosshandObj);
    hand = handweakptr;
}

void Boss::DestroyHand(){
    if(!hand.expired()){
        hand.lock()->RequestDelete();
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

void Boss::HitEffect(std::string file,int frames,float frametime,float duration,Vec2 point,bool flip,Vec2 offset){
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

int Boss::GetHP(){
    return hp;
}

Boss::bossState Boss::GetState(){
    return state;
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
