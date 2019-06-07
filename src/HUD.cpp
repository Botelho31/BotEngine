#include "../include/HUD.h"
#include "../include/CameraFollower.h"
#include "../include/Player.h"

HUD::HUD(GameObject& associated) : Component(associated){
    this->belicon = new GameObject();
    belicon->renderAfterForeGround = true;
    Sprite *belIcon = new Sprite(*belicon,"assets/img/HUD/belicon.png");
    belicon->box.Transform(50,20);
    CameraFollower *camfollower =  new CameraFollower(*belicon);
    belicon->AddComponent(camfollower);
    belicon->AddComponent(belIcon);

    bar = new GameObject();
    bar->renderAfterForeGround = true;
    Sprite *belIcon2 = new Sprite(*bar,"assets/img/HUD/barraleatoria.png");
    bar->box.Transform(300,175);
    CameraFollower *camfollower2 =  new CameraFollower(*bar);
    bar->AddComponent(camfollower2);
    bar->AddComponent(belIcon2);

    GameObject *ownerObj = new GameObject();
    std::vector<std::string> lifesprites;
    lifesprites.push_back("assets/img/HUD/vida.png");
    lifesprites.push_back("assets/img/HUD/morte.png");
    life = new TileSet(ownerObj,77,77,lifesprites);
}

HUD::~HUD(){
    delete life;
    delete belicon;
    delete bar;
}

void HUD::Update(float dt){
    belicon->Update(dt);
    bar->Update(dt);
}

void HUD::Render(){
    belicon->Render();
    bar->Render();
    int numberoficons = Player::player->GetLife()/10;
    int currentWidth = 300;
    int height = 100;
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