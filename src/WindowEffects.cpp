#include "../include/WindowEffects.h"
#include "../include/Camera.h"

std::vector<WindowEffects::BoxClass> WindowEffects::drawqueue;

WindowEffects::WindowEffects(){
    currentEffect = NOTHING;
    currenteffectseconds = 0;
}

WindowEffects::~WindowEffects(){

}

void WindowEffects::Update(float dt){
    float alphapersecond = 0;
    switch(currentEffect){
        case FADETOBLACK:
            alphapersecond = 255/currenteffectseconds;
            if((a + (alphapersecond *dt)) >= 255){
                a = 255;
                currenteffectseconds = 0;
            }else{
                a += alphapersecond * dt;
            }
            DrawToScreen(r,g,b,a);
            break;
        case FADEFROMBLACK:
            alphapersecond = -(255/currenteffectseconds);
            if((a + (alphapersecond *dt)) <= 0){
                a = 0;
                currentEffect = NOTHING;
                currenteffectseconds = 0;
            }else{
                a += alphapersecond * dt;
            }
            DrawToScreen(r,g,b,a);
            break;
        case NOTHING:
            break;
        default:
            break;
    }
}

void WindowEffects::Render(){
    if(Drawing()){
        DrawToScreen(r,g,b,a);
    }
    for(int i = 0;i < drawqueue.size(); i++){
        DrawBox(drawqueue[i].box,drawqueue[i].angle,drawqueue[i].r,drawqueue[i].g,drawqueue[i].b);
    }
    drawqueue.clear();
}

void WindowEffects::Reset(){
    currentEffect = NOTHING;
    currenteffectseconds = 0;
}

void WindowEffects::FadeToBlack(int seconds){
    r = 0;
    g = 0;
    b = 0;
    a = 0;
    this->currenteffectseconds = seconds;
    this->currentEffect = FADETOBLACK;
}

void WindowEffects::FadeFromBlack(int seconds){
    r = 0;
    g = 0;
    b = 0;
    a = 255;
    this->currenteffectseconds = seconds;
    this->currentEffect = FADEFROMBLACK;
}

void WindowEffects::DrawToScreen(int r,int g,int b,int a){
    SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(),SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(),r,g,b,a);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = Camera::window.x;
    rect.h = Camera::window.y;
    SDL_RenderFillRect(Game::GetInstance().GetRenderer(),&rect);
}

WindowEffects::Effect WindowEffects::GetCurrentEffect(){
    return currentEffect;
}

bool WindowEffects::Drawing(){
    if(currentEffect == NOTHING){
        return false;
    }else{
        return true;
    }
}

bool WindowEffects::IsBlack(){
    if((a == 255) && (r == 0) && (g == 0) && (b == 0)){
        return true;
    } else{
        return false;
    }
}

void WindowEffects::DrawBox(Rect box,float angle,int r,int g,int b){
        Vec2 center( box.GetCenter() );
		SDL_Point points[5];

        Vec2 point = (Vec2(box.x, box.y) - center).GetRotated( angle )
						+ center - Camera::pos;
		points[0] = {(int)point.x, (int)point.y};
		points[4] = {(int)point.x, (int)point.y};
		
		point = (Vec2(box.x + box.w, box.y) - center).GetRotated( angle )
						+ center - Camera::pos;
		points[1] = {(int)point.x, (int)point.y};
		
		point = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated( angle )
						+ center - Camera::pos;
		points[2] = {(int)point.x, (int)point.y};
		
		point = (Vec2(box.x, box.y + box.h) - center).GetRotated( angle )
						+ center - Camera::pos;
		points[3] = {(int)point.x, (int)point.y};

		SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
}

void WindowEffects::AddBoxToDraw(Rect box,float angle,int r,int g,int b){
    BoxClass *boxdraw = new BoxClass(box,angle,r,g,b);
    drawqueue.emplace_back(*boxdraw);
}