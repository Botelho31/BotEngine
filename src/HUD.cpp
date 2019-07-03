#include "../include/HUD.h"
#include "../include/CameraFollower.h"
#include "../include/Player.h"
#include "../include/GameData.h"

HUD::HUD(GameObject& associated) : Component(associated){
    this->belicon = new GameObject();
    belicon->renderAfterForeGround = true;
    Sprite *belIcon = new Sprite(*belicon,"assets/img/HUD/belicon2.png");
    belicon->box.Transform(50,20);
    CameraFollower *camfollower =  new CameraFollower(*belicon);
    belicon->AddComponent(camfollower);
    belicon->AddComponent(belIcon);

    bar = new GameObject();
    bar->renderAfterForeGround = true;
    this->manabar = new StatBar(*bar,"assets/img/HUD/barraleatoria2.png","assets/img/HUD/barraleatoria2.png",{38,37},10,PLAYERMANA);
    manabar->SetCurrent(PLAYERMANA,false);
    // Sprite *belIcon2 = new Sprite(*bar,"assets/img/HUD/barraleatoria2.png");
    bar->box.Transform(230,145);
    CameraFollower *camfollower2 =  new CameraFollower(*bar);
    bar->AddComponent(camfollower2);
    bar->AddComponent(manabar);

    GameObject *ownerObj = new GameObject();
    std::vector<std::string> lifesprites;
    lifesprites.push_back("assets/img/HUD/vida2.png");
    lifesprites.push_back("assets/img/HUD/morte2.png");
    life = new TileSet(ownerObj,60,60,lifesprites);
}

HUD::~HUD(){
    delete life;
    delete belicon;
    delete bar;
    manabar = nullptr;
}

void HUD::Update(float dt){
    if(manabar->GetCurrent() != GameData::savePlayerMana){
        manabar->SetCurrent(GameData::savePlayerMana,false);
    }
    belicon->Update(dt);
    bar->Update(dt);
}

void HUD::Render(){
    belicon->Render();
    bar->Render();
    int numberoficons = Player::player->GetLife()/10;
    int currentWidth = 230;
    int height = 60;
    for(int i = 0;i < numberoficons;i++){
        life->RenderTile(0,currentWidth,height);
        currentWidth += life->GetTileWidth();
    }
    for(int i = 0;i < ((PLAYERHP/10) - numberoficons);i++){
        life->RenderTile(1,currentWidth,height);
        currentWidth += life->GetTileWidth();
    }
}

bool HUD::Is(std::string type){
    if(type == "HUD"){
        return true;
    }else{
        return false;
    }
}