#include "../include/TileCollider.h"
#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/TileMap.h"

TileCollider::TileCollider(GameObject& associated,Rect tilebox) : 
    Component(associated),box(tilebox){
    maxX = false;
	maxY = false;
 	deleted = false;
	left = false;
	right = false;
	up = false;
	down = false;
	associated.box = tilebox;
}

void TileCollider::Start(){
}

void TileCollider::Update(float dt){
	if(!maxX || !maxY){
		box = associated.box;
		Vec2 Edges[] = {   Vec2(box.x + box.w/2,box.y + box.h + 1),
							Vec2(box.x + box.w/2,box.y - 1),
							Vec2(box.x + box.w + 1,box.y + box.h/2),
							Vec2(box.x - 1,box.y + box.h/2),
		};
		if(CanMove(Edges[0])){
			down = true;
		}
		if(CanMove(Edges[1])){
			up = true;
		}
		if(CanMove(Edges[2])){
			right = true;
		}
		if(CanMove(Edges[3])){
			left = true;
		}
		if((right) && (left)){
			maxX = true;
		}
		if((up) && (down)){
			maxY = true;
		}
		std::cout << "Not concluded " << box.x << " " << box.y << std::endl;
		if(!up){
			std::cout << "up not" << std::endl;
		}
		if(!down){
			std::cout << "down not" << std::endl;
		}
		if(!left){
			std::cout << "left not" << std::endl;
		}
		if(!right){
			std::cout << "right not" << std::endl;
		}
	}else{
		// Collider *collider = new Collider(associated);
		// associated.AddComponent(collider);
		// associated.RemoveComponent(this);
	}
}

bool TileCollider::CanMove(Vec2 vector){
    TileMap *tilemap = Game::GetInstance().GetCurrentState().GetTileMap();
    if(tilemap->AtLocation(vector.x,vector.y,true) > -1){
        return false;
    }else{
        return true;
    }
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

void TileCollider::NotifyCollision(GameObject& other){
	if(!deleted && (!maxX || !maxY)){
		Component *tilecomponent = other.GetComponent("TileCollider");
		if(tilecomponent){
			TileCollider *tilecollider = dynamic_cast<TileCollider*>(tilecomponent);
			if(!tilecollider->deleted){
				if(!tilecollider->maxX){
					if((other.box.y == associated.box.y) && (other.box.h == associated.box.h)){
						associated.box.w += other.box.w;
						if(other.box.x < associated.box.x){
							associated.box.x = other.box.x;
						}
						tilecollider->deleted = true;
						other.RequestDelete();
						// KeepStill(true,0.5);
					}
				}else if(!tilecollider->maxY){
					if((other.box.x == associated.box.x) && (other.box.w == associated.box.w)){
						associated.box.h += other.box.h;
						if(other.box.y < associated.box.y){
							associated.box.y = other.box.y;
						}
						tilecollider->deleted = true;
						other.RequestDelete();
						// KeepStill(true,0.5);
					}
				}
				if(tilecollider->maxX){
					if(tilecollider->box.x <= associated.box.x){
						left = true;
					}else if(tilecollider->box.x >= associated.box.x + associated.box.w){
						right = true;
					}
				}
				if(tilecollider->maxY){
					if(tilecollider->box.y <= associated.box.y){
						up = true;
					}else if(tilecollider->box.y >= associated.box.y + associated.box.h){
						down = true;
					}
				}
			}
		}
	}
}

bool TileCollider::Is(std::string type){
    if(type == "TileCollider"){
        return true;
    }else{
        return false;
    }
}
