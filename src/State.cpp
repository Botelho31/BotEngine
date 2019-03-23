#include "../include/State.h"
#include "../include/Face.h"
#include "../include/Vec2.h"
#include "../include/Sound.h"

State::State(){
    quitRequested = false;
    music = new Music("assets/audio/stageState.ogg");
    music->Play();
    // bg = new Sprite("assets/img/ocean.jpg");
    // bg->SetClip(0,0,bg->GetWidth(),bg->GetHeight());
    // bg->Render(0,0);
}

State::~State(){
    objectArray.clear();
}

void State::Input(){
    SDL_Event event;
	int mouseX, mouseY;

	// Obtenha as coordenadas do mouse
	SDL_GetMouseState(&mouseX, &mouseY);

	// SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
	while (SDL_PollEvent(&event)) {

		// Se o evento for quit, setar a flag para terminação
		if(event.type == SDL_QUIT) {
			quitRequested = true;
		}
		
		// Se o evento for clique...
		if(event.type == SDL_MOUSEBUTTONDOWN) {

			// Percorrer de trás pra frente pra sempre clicar no objeto mais de cima
			for(int i = objectArray.size() - 1; i >= 0; --i) {
				// Obtem o ponteiro e casta pra Face.
				GameObject* go = (GameObject*) objectArray[i].get();
				// Nota: Desencapsular o ponteiro é algo que devemos evitar ao máximo.
				// O propósito do unique_ptr é manter apenas uma cópia daquele ponteiro,
				// ao usar get(), violamos esse princípio e estamos menos seguros.
				// Esse código, assim como a classe Face, é provisório. Futuramente, para
				// chamar funções de GameObjects, use objectArray[i]->função() direto.

				if(go->box.Contains( (float)mouseX, (float)mouseY ) ) {
					Face* face = (Face*)go->GetComponent( "Face" );
					if ( nullptr != face ) {
						// Aplica dano
						face->Damage(std::rand() % 10 + 10);
						// Sai do loop (só queremos acertar um)
						break;
					}
				}
			}
		}
		if( event.type == SDL_KEYDOWN ) {
			// Se a tecla for ESC, setar a flag de quit
			if( event.key.keysym.sym == SDLK_ESCAPE ) {
				quitRequested = true;
			}
			// Se não, crie um objeto
			else {
				Vec2 objPos = Vec2( 200, 0 ).GetRotated( -PI + PI*(rand() % 1001)/500.0 ) + Vec2( mouseX, mouseY );
				AddObject((int)objPos.x, (int)objPos.y);
			}
		}
	}
}

void State::AddObject(int mouseX,int mouseY){
    GameObject *object = new GameObject();
    Sprite *penguin  =  new Sprite(*object,"../assets/img/penguinface.png");
    object->AddComponent(penguin);
    object->box.x = mouseX - (object->box.w/2);
    object->box.y = mouseY - (object->box.h/2);
    Sound *sound = new Sound(*object,"../assets/audio/boom.wav");
    object->AddComponent(sound);
    Face *face = new Face(*object);
    object->AddComponent(face);
    objectArray.emplace_back(object);
}

void State::LoadAssets(){

}

void State::Update(float dt){
    Input();
    float t = 0;
    for(unsigned int i = 0; i <= objectArray.size();i++){
        objectArray[i]->Update(t);
    }
    for(unsigned int i = 0; i <= objectArray.size();i++){
        if(objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin() + i);
        }
    }
}

void State::Render(){
    for(unsigned int i = 0; i <= objectArray.size();i++){
        objectArray[i]->Render();
    }
}

bool State::QuitRequested(){
    return quitRequested;
}