#include "../include/TileCollider.h"
#include "../include/Camera.h"
#include "../include/InputManager.h"

TileCollider::TileCollider(GameObject& associated,Rect tilebox) : 
    Component(associated),box(tilebox){
    max = false;
}

void TileCollider::Start(){
}

void TileCollider::Update(float dt){
    associated.box = box;
}


void TileCollider::Render() {
#ifdef DEBUG
	InputManager *input = &(InputManager::GetInstance());
	if(input->IsKeyDown(SDLK_EQUALS)){
		Vec2 center( box.GetCenter() );
		SDL_Point points[5];

		Vec2 point = (Vec2(box.x, box.y) - center).GetRotated( associated.angleDeg/(180/PI) )
						+ center - Camera::pos;
		points[0] = {(int)point.x, (int)point.y};
		points[4] = {(int)point.x, (int)point.y};
		
		point = (Vec2(box.x + box.w, box.y) - center).GetRotated( associated.angleDeg/(180/PI) )
						+ center - Camera::pos;
		points[1] = {(int)point.x, (int)point.y};
		
		point = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated( associated.angleDeg/(180/PI) )
						+ center - Camera::pos;
		points[2] = {(int)point.x, (int)point.y};
		
		point = (Vec2(box.x, box.y + box.h) - center).GetRotated( associated.angleDeg/(180/PI) )
						+ center - Camera::pos;
		points[3] = {(int)point.x, (int)point.y};

		SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
	}
#endif // DEBUG
}

bool TileCollider::Is(std::string type){
    if(type == "TileCollider"){
        return true;
    }else{
        return false;
    }
}
