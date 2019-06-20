
#include "../include/Spit.h"
#include "../include/Collider.h"
#include "../include/Player.h"
#include "../include/HitBox.h"
#include "../include/Camera.h"
#include "../include/WindowEffects.h"
#include "../include/DeadBody.h"
#include "../include/GameData.h"

Spit::Spit(GameObject& associated, GameObject& spitOwner, float initSpeed) : Component(associated){
 
	speed.x = initSpeed * cos(associated.angleDeg * (2*3.14/360)) * 1.5;
	speed.y = initSpeed * sin(associated.angleDeg * (2*3.14/360));
	std::cout<<"Vel x: "<<speed.x<<"; Vel y: "<<speed.y<<std::endl;
	this->physics = new Physics(associated,&speed,false,false);
	associated.AddComponent(physics);


	gravspeed = 600;


	spritefiles = GameData::GetSpritesFiles("assets/img/info/spit.txt");
    Sprite *spitSprite =  new Sprite(associated,spritefiles["spit"],4,0.1);
    this->spitsprite = spitSprite;
    associated.AddComponent(spitSprite);

    physics->SetCollider(0.5,0.65,0,0);

    splashTimer = new Timer();
    puddleTimer = new Timer();
    collided = false;
}

Spit::~Spit(){
	physics = nullptr;
	spitsprite = nullptr;
}

void Spit::Update(float dt){


	physics->PerformGravity(gravspeed, dt);

	associated.angleDeg = Vec2(0,0).GetAngle(speed) * 360 / (2*3.14);

	physics->PerformYMovement(dt);
	physics->PerformXMovement(dt);

	if(physics->IsColliding() || physics->IsGrounded() || physics->IsRight()  || physics->IsLeft()){
		collided=true;



	}


	if(collided){
    	//std::cout<<"AAAA\nAAAA\n\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA"<<std::endl;

		if(physics->IsGrounded()){	//Create DeadBody (spit puddle) only if it is on the ground
			GameObject *puddleObj = new GameObject();
			Sprite *puddleSprite = new Sprite(*puddleObj,spritefiles["spitSplashing"],4,0.1,0,false,true);
        	DeadBody *puddle = new DeadBody(*puddleObj,Vec2(0,0),puddleSprite,Vec2(0.5,0.35),Vec2(0,0),false);
        	puddleObj->AddComponent(puddle);
    		puddleObj->box.SetCenter(associated.box.GetCenter().Added(0,0));
        	Game::GetInstance().GetCurrentState().AddObject(puddleObj);
        	associated.RequestDelete();
		}
		else{			//Create a simple splash if spit did not hit the ground
			if(!splashTimer->Started()){
				
				SetSprite(spritefiles["spitSplashing"], 4, 0.1, false);
				gravspeed = 0;

				

			}
			splashTimer->Update(dt);
			if(splashTimer->Get() >= 0.4){
				associated.RequestDelete();
			}
		}

		/*
		if(!splashTimer->Started()){
			SetSprite(spritefiles["spitSplashing"], 4, 0.1, false);
			speed = Vec2(0,0);
			gravspeed = 0;
		}
		splashTimer->Update(dt);
		if(splashTimer->Get() >= 0.4){
			if(!puddleTimer->Started()){
				SetSprite(spritefiles["spitPuddle"], 4, 0.1, false);
			}
			puddleTimer->Update(dt);
			if(puddleTimer->Get() > 5){
				associated.RequestDelete();
			}
			
		}*/
		
	}

}


void Spit::Render(){}

bool Spit::Is(std::string type){
    if(type == "Spit"){
        return true;
    }else{
        return false;
    }
}

void Spit::Start(){}

void Spit::NotifyCollision(GameObject& other){
 	Component *component1 = other.GetComponent("Player");
    //std::cout<<"TO SETANDO ESSA MERDA DEPOIS DE BATER, CARAI"<<std::endl;
    
    if(component1){
        //HitBox *hitbox = dynamic_cast<HitBox*>(component1);
    	collided = true;

        /*if((hitbox)){//  && hitbox->HitEnemy()){
            physics->KnockBack(hitbox->GetOwner()->box,hitbox->GetKnockBack());
            if(hitbox->GetOwner()->box.GetCenter().x < GetPosition().x){
                if(!minionsprite->IsFlipped()){
                    minionsprite->Flip();
                }
            }else{
                if(minionsprite->IsFlipped()){
                    minionsprite->Flip();
                }
            }
            DamageFlyingMinion(hitbox->GetDamage());
            damageCooldown = hitbox->GetDamageCooldown();
            invincibilitytimer->Delay(0);
        }*/

    }
    /*if(component2){
    	std::cout<<"AAAA\nAAAA\n\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA\nAAAA"<<std::endl;
        //physics->KnockBack(other.box,{5,0},true,{600,400});
    }*/
 }

void Spit::SetSprite(std::string file,int framecount,float frametime,bool repeat,Vec2 offset){
    Rect prepos = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    spitsprite->SetFrameCount(framecount);
    spitsprite->SetFrameTime(frametime);
    spitsprite->SetRepeat(repeat);
    spitsprite->Open(file);
    associated.box.x = prepos.x + (prepos.w/2) - (associated.box.w/2) + offset.x;
    associated.box.y = prepos.y + (prepos.h/2) - (associated.box.h/2) + offset.y;
    physics->GetCollider()->UpdateScale();
}