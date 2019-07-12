#include "../include/Velho.h"
#include "../include/CameraFollower.h"
#include "../include/GameData.h"

Velho::Velho(GameObject& associated) : Component(associated){

    state = IDLE;
    Sprite *sprite = new Sprite(associated,"assets/img/velho.png",16,0.16,false);
    associated.AddComponent(sprite);

    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    texts.push_back("O meu filho, ainda bem que voce veio falar comigo.");
    texts.push_back("Eu sou o dono da vendinha oficial de MNUNJGAKALOK, DEUS DO SUBMUNDO.");
    texts.push_back("Eu trabalho aqui faz aproximadamente 5037 anos, mas ultimamente meio que dei uma cansada, sabe?");
    texts.push_back("Nao ando recebendo muito bem, e eles nao tem plano dentario bom.");
    texts.push_back("Acho que tava na hora de alguem matar ele logo mesmo.");
    texts.push_back("Se voce tivesse ido direto na direcao dele, com ABSOLUTA CERTEZA ele ia te engolir");
    texts.push_back("Ja vi muita gente fazendo essa burrice, muita gente mesmo.");
    texts.push_back("Ainda bem que voce nao e panaca desse jeito.");
    texts.push_back("...");
    texts.push_back("Mas enfim se voce tem qualquer interesse em vencer o MNUNJGAKALOK, DEUS DO SUBMUNDO voce vai precisar invocar um Excaliberto pra isso");
    texts.push_back("Voce nao sabe como conseguir um Excaliberto?");
    texts.push_back("Ok, vou te explicar:");
    texts.push_back("MNUNJGAKALOK mantem varias almas aprisionadas nas catacumbas aqui embaixo, nesse buraco.");
    texts.push_back("Durante muitos anos ele prendeu as almas perdidas dos habitantes da Terra pra aumentar seu poder.");
    texts.push_back("Nao soa muito legal, mas confie em mim, da dinheiro mesmo.");
    texts.push_back("Se voce conseguir salvar almas o bastante, elas podem topar te ajudar.");
    texts.push_back("Juntando a energia de varias voce consegue invocar o Escaliberto, a unica espada capaz de matar um demonio.");
    texts.push_back("Bem facil, ne?");
    texts.push_back("Agora pode ir la que ta na minha hora de soneca.");
    texts.push_back("Zzzzzz");
    
    texts2.push_back("Opa! Voltou, meu filho?");
    texts2.push_back("Eu esqueci de te avisar que voce ia precisar da invocacao do pulo duplo pra voltar");
    texts2.push_back("Mas acho que voce conseguiu resolver isso sozinho, nao e?");
    texts2.push_back("Boa.");
    texts2.push_back("Agora sim acho que voce tem a mínima chance de vencer MNUNJGAKALOK, DEUS DO SUBMUNDO");
    texts2.push_back("Boa sorte!");

    if(GameData::playerDoubleJump){
        texts = texts2;
    }


}

Velho::~Velho(){
}

void Velho::Start(){

}

void Velho::Velho::Update(float dt){
    InputManager *input =  &(InputManager::GetInstance());
    if(state == CHATTING){
        if(input->KeyPress(SDLK_f)){
            currenttext ++;
            if(currenttext >= texts.size()){
                state = IDLE;
                RemoveChatBox();
            }else{
                text->SetText(texts[currenttext]);
            }
        }
    }
}

void Velho::AddChatBox(){
    this->linhacaixadetexto =  new GameObject();
    Sprite *sprite2 = new Sprite(*linhacaixadetexto,"assets/img/HUD/velho/barra.png");
    linhacaixadetexto->renderAfterForeGround = true;
    linhacaixadetexto->AddComponent(sprite2);
    linhacaixadetexto->box.Transform(0,800);
    CameraFollower *camfollower = new CameraFollower(*linhacaixadetexto);
    linhacaixadetexto->AddComponent(camfollower);
    Game::GetInstance().GetCurrentState().AddObject(linhacaixadetexto);

    this->caixadetexto =  new GameObject();
    Sprite *sprite3 = new Sprite(*caixadetexto,"assets/img/HUD/velho/retangulo.png");
    caixadetexto->renderAfterForeGround = true;
    caixadetexto->AddComponent(sprite3);
    caixadetexto->box.Transform(0,800);
    CameraFollower *camfollower2 = new CameraFollower(*caixadetexto);
    caixadetexto->AddComponent(camfollower2);
    Game::GetInstance().GetCurrentState().AddObject(caixadetexto);

    this->textObj = new GameObject();
    this->text = new Text(*textObj,"assets/font/Rosario-Regular.ttf",40,Text::BLENDED,texts[0],{255,255,255});
    textObj->box.Transform(100,850);
    this->textObj->renderAfterForeGround = true;
    textObj->AddComponent(text);
    CameraFollower *camfollower3 = new CameraFollower(*textObj);
    textObj->AddComponent(camfollower3);
    Game::GetInstance().GetCurrentState().AddObject(textObj);

    currenttext = 0;
}

void Velho::RemoveChatBox(){
    linhacaixadetexto->RequestDelete();
    caixadetexto->RequestDelete();
    textObj->RequestDelete();
    text = nullptr;
}

void Velho::Render(){
}

bool Velho::Is(std::string type){
    if(type == "Velho"){
        return true;
    }else{
        return false;
    }
}

void Velho::NotifyCollision(GameObject& other){
    if(other.GetComponent("Player")){
        if(state != CHATTING){
            state = CHATTING;
            AddChatBox();
        }
    }
}