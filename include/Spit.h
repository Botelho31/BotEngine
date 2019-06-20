#ifndef GAME_H
    #include "Game.h"
#endif


#ifndef SPIT_H
#define SPIT_H


class Spit : public Component{
public:
	Spit(GameObject& associated, GameObject& spitOwner, float initSpeed);
	~Spit();

	void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject& other);
    void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0});

private:
	Sprite *spitsprite;
	Physics *physics;
	int gravspeed;
	Vec2 speed;

	bool collided;
	Timer *splashTimer;
	Timer *puddleTimer;


	std::map<std::string,std::string> spritefiles;
};



#endif