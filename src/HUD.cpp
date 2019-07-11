#include "../include/HUD.h"
#include "../include/CameraFollower.h"
#include "../include/Player.h"
#include "../include/GameData.h"
#include "../include/Boss.h"

HUD::HUD(GameObject& associated) : Component(associated){
    this->belicon = new GameObject();
    belicon->renderAfterForeGround = true;
    Sprite *belIcon = new Sprite(*belicon,"assets/img/HUD/belicon2.png");
    belicon->box.Transform(50,20);
    CameraFollower *camfollower =  new CameraFollower(*belicon);
    belicon->AddComponent(camfollower);
    belicon->AddComponent(belIcon);

    this->healthicon = new GameObject();
    healthicon->renderAfterForeGround = true;
    Sprite *healthIcon = new Sprite(*healthicon,"assets/img/HUD/coracao.png");
    healthicon->box.Transform(255,65);
    CameraFollower *camfollower2 =  new CameraFollower(*healthicon);
    healthicon->AddComponent(camfollower2);
    healthicon->AddComponent(healthIcon);

    this->manaicon = new GameObject();
    manaicon->renderAfterForeGround = true;
    Sprite *manaIcon = new Sprite(*manaicon,"assets/img/HUD/raio.png");
    manaicon->box.Transform(255,130);
    CameraFollower *camfollower3 =  new CameraFollower(*manaicon);
    manaicon->AddComponent(camfollower3);
    manaicon->AddComponent(manaIcon);

    manabarObj = new GameObject();
    manabarObj->renderAfterForeGround = true;
    this->manabar = new StatBar(*manabarObj,"assets/img/HUD/bars/energiavazio.png","assets/img/HUD/bars/energiacheio.png",{6,88},PLAYERMANA);
    manabar->SetCurrent(PLAYERMANA,false);
    manabarObj->box.Transform(340,120);
    CameraFollower *camfollower4 =  new CameraFollower(*manabarObj);
    manabarObj->AddComponent(camfollower4);
    manabarObj->AddComponent(manabar);

    healthbarObj = new GameObject();
    healthbarObj->renderAfterForeGround = true;
    this->healthbar = new StatBar(*healthbarObj,"assets/img/HUD/bars/vidavazio.png","assets/img/HUD/bars/vidacheio.png",{6,49},PLAYERHP);
    healthbar->SetCurrent(PLAYERHP,false);
    healthbarObj->box.Transform(340,75);
    CameraFollower *camfollower5 =  new CameraFollower(*healthbarObj);
    healthbarObj->AddComponent(camfollower5);
    healthbarObj->AddComponent(healthbar);

    bossbarObj = new GameObject();
    bossbarObj->renderAfterForeGround = true;
    this->bossbar = new StatBar(*bossbarObj,"assets/img/HUD/bars/bossvazio.png","assets/img/HUD/bars/bosscheio.png",{10,124},BOSSHP);
    bossbar->SetCurrent(PLAYERMANA,false);
    bossbarObj->box.Transform(345,925);
    CameraFollower *camfollower6 =  new CameraFollower(*bossbarObj);
    bossbarObj->AddComponent(camfollower6);
    bossbarObj->AddComponent(bossbar);
}

HUD::~HUD(){
    delete healthicon;
    delete manaicon;
    delete healthbarObj;
    delete belicon;
    delete manabarObj;
    manabar = nullptr;
    bossbar = nullptr;
    healthbar = nullptr;
}

void HUD::Update(float dt){
    if(manabar->GetCurrent() != GameData::savePlayerMana){
        manabar->SetCurrent(GameData::savePlayerMana,false);
    }
    if(Player::player){
        if(healthbar->GetCurrent() != Player::player->GetLife()){
            healthbar->SetCurrent(Player::player->GetLife(),false);
        }
    }
    if(Boss::boss){
        bossbar->SetCurrent(Boss::boss->GetHP(),false);
    }
    belicon->Update(dt);
    manabarObj->Update(dt);
    manaicon->Update(dt);

    healthicon->Update(dt);
    healthbarObj->Update(dt);

    bossbarObj->Update(dt);
}

void HUD::Render(){
    belicon->Render();
    manabarObj->Render();
    healthicon->Render();
    healthbarObj->Render();
    manaicon->Render();
    if(Boss::boss){
        bossbarObj->Render();
    }
}

bool HUD::Is(std::string type){
    if(type == "HUD"){
        return true;
    }else{
        return false;
    }
}