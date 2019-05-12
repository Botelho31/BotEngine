#include "../include/TileCollider.h"
#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/TileMap.h"

TileCollider::TileCollider(GameObject& associated,Rect tilebox,bool moving) : 
    Component(associated),box(tilebox){
	this->moving = moving;
	if(moving){
		maxX = true;
		maxY = true;
		up = true;
		down = true;
		left = true;
		right = true;

	}else{
		maxX = false;
		maxY = false;
		up = false;
		down = false;
		left = false;
		right = false;
	}
 	deleted = false;
	associated.box = tilebox;
}

void TileCollider::Start(){
}

void TileCollider::Update(float dt){
	box = associated.box;
	if(!maxX || !maxY){
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
	}
}

bool TileCollider::CanMove(Vec2 vector){
    TileMap *tilemap = Game::GetInstance().GetCurrentState().GetTileMap();
    if(tilemap->AtLocation(vector.x,vector.y) > -1){
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

void TileCollider::NotifyMobCollision(GameObject& other){
	if(moving){
		Physics *physics1 = other.GetPhysics();
		if(physics1){
			Collider *collider = physics1->GetCollider();
			float distground,distceiling,distright,distleft;
			distright = collider->box.x - (associated.box.x + associated.box.w);
			distleft = associated.box.x - (collider->box.x + collider->box.w);
			distceiling = collider->box.y - (associated.box.y + associated.box.h);
			distground = associated.box.y - (collider->box.y + collider->box.h);
			if( (distright != 0) && (distleft != 0) && (distright != 0) && (distleft != 0)){
				std::map<int,int> dists;
				dists.insert({0,distground});
				dists.insert({1,distceiling});
				dists.insert({2,distright});
				dists.insert({3,distleft});
				std::deque<int> disttofix;
				for(int i = 0;i < 4;i++){
					if(dists[i] < 0){
						disttofix.push_front(i);
					}
				}
				bool inserted = true;
				while(inserted){
					inserted = false;
					for(unsigned int i = 0;i < disttofix.size();i++){
						if(i != (disttofix.size() -1)){
							if(dists[disttofix[i]] < dists[disttofix[i + 1]]){
								int a = disttofix[i];
								disttofix[i] = disttofix[i + 1];
								disttofix[i + 1] = a;
								inserted = true;
							}
						}
					}
				}
				for(int i = 0;i < disttofix.size();i++){
					std::cout << i << " " << disttofix[i] << " " << dists[disttofix[i]] << std::endl;   
				}
				if(!disttofix.empty()){
					if(disttofix[0] == 0){
						associated.box.y += distground - 1;
					}
					if(disttofix[0] == 1){
						associated.box.y -= distceiling - 1;
					}
					if(disttofix[0] == 2){
						associated.box.x += distright - 1;
					}
					if(disttofix[0] == 3){
						associated.box.x -= distleft - 1;
					}
				}

			}			
			std::cout << "dground: "<< distground << std::endl;
            std::cout << "dceiling: "<< distceiling << std::endl;
            std::cout << "dright: "<< distright << std::endl;
            std::cout << "dleft: "<< distleft << std::endl;
			box = associated.box;
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
