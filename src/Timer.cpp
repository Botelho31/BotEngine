#include "../include/Timer.h"

Timer::Timer(){
    time = 0;
    started = false;
}

void Timer::Update(float dt){
    time += dt;
    started = true;
}

void Timer::Delay(float dt){
    time -= dt;
    started = true;
}

void Timer::Restart(){
    time = 0;
    started = false;
}

float Timer::Get(){
    return time;
}

bool Timer::Started(){
    return started;
}