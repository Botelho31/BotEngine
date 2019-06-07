#include "../include/HUD.h"
#include "../include/CameraFollower.h"
#include "../include/Player.h"

HUD::HUD(GameObject& associated) : Component(associated){
    GameObject *belIconObj = new GameObject();
    belIconObj->renderAfterForeGround = true;
    Sprite *belIcon = new Sprite(*belIconObj,"assets/img/HUD/belicon.png");
    belIconObj->box.Transform(50,20);
    CameraFollower *camfollower =  new CameraFollower(*belIconObj);
    belIconObj->AddComponent(camfollower);
    belIconObj->AddComponent(belIcon);
    belicon = Game::GetInstance().GetCurrentState().AddObject(belIconObj);

    GameObject *belIconObj2 = new GameObject();
    belIconObj2->renderAfterForeGround = true;
    Sprite *belIcon2 = new Sprite(*belIconObj2,"assets/img/HUD/barraleatoria.png");
    belIconObj2->box.Transform(300,175);
    CameraFollower *camfollower2 =  new CameraFollower(*belIconObj2);
    belIconObj2->AddComponent(camfollower2);
    belIconObj2->AddComponent(belIcon2);
    bar = Game::GetInstance().GetCurrentState().AddObject(belIconObj2);

    GameObject *ownerObj = new GameObject();
    std::vector<std::string> lifesprites;
    lifesprites.push_back("assets/img/HUD/vida.png");
    lifesprites.push_back("assets/img/HUD/morte.png");
    life = new TileSet(ownerObj,77,77,lifesprites);
}

HUD::~HUD(){
    delete life;
}

void HUD::Update(float dt){
}

void HUD::Render(){
    int numberoficons = Player::player->GetLife()/10;
    int currentWidth = 300;
    int height = 100;
    for(int i = 0;i < numberoficons;i++){
        life->RenderTile(0,currentWidth,height);
        currentWidth += life->GetTileWidth();
    }
    for(int i = 0;i < ((PLAYERHP/10) - numberoficons);i++){
        life->RenderTile(1,currentWidth,height,true);
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