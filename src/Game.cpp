#include "../include/Game.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"

Game* Game::instance;

Game& Game::GetInstance(){
    if(instance != nullptr){
        return *instance;
    }
    else{
        Game *game = new Game("BotEngine",960,540); ////
        return *game;
    }
}

Game::Game(std::string Title,int Width,int Height){
    srand(time(NULL));
    if(instance != nullptr){
        std::cout << "Fatal Error: Two instances being created\n" << std::endl;
        return;
    }
    else{
        instance = this;
        frameStart = 0;
        dt = 0;
        Camera::window.x = Width;
        Camera::window.y = Height;
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0){
            std::cout << "Game instance Created\n" << std::endl;
            std::cout << "Number of Img Libraries Initialized: " << IMG_Init(IMG_INIT_JPG  |  IMG_INIT_PNG  |  IMG_INIT_TIF) << "\n" << std::endl;
            if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024) == 0){
                std::cout << "Mix_OpenAudio Initialized\n" << std::endl;
            }else{
                std::cout << "Fatal Error Loading OpenAudio: " << SDL_GetError() << "\n" << std::endl; 
            }
            std::cout << "Number of Mix Libraries Initialized: " << Mix_Init(MIX_INIT_MP3) << "\n" << std::endl;
            if(TTF_Init() == 0){
                std::cout << "TTF Initialized" << "\n" <<  std::endl;
            }else{
                std::cout << "Fatal Error Loading TTF: " << SDL_GetError() << "\n" << std::endl; 
            }
            Mix_AllocateChannels(32);
            window = SDL_CreateWindow(Title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,Width,Height,0); 
            if(window){
                std::cout << "Window Initialized\n" << std::endl;
            }else{
                std::cout << "Fatal Error Loading Window: " << SDL_GetError() << "\n" << std::endl; 
            }
            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
            if(renderer){
                std::cout << "Renderer Initialized\n" << std::endl;
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
                SDL_RenderSetLogicalSize(renderer,1920,1080);
                Camera::window.x = 1920;
                Camera::window.y = 1080;

                // SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
            }else{
                std::cout << "Fatal Error Loading Renderer: " << SDL_GetError() << "\n" << std::endl; 
                std::cout << "Number of Drivers: " << SDL_GetNumRenderDrivers() << "\n" << std::endl;
                SDL_RendererInfo info;
                SDL_GetRendererInfo(renderer,&info);
                std::cout << "Driver in use: " << info.name << "\n" << std::endl;
            }
            resources = new Resources();
            storedstate = nullptr;
            

        }else{
            std::cout << "Fatal Error: SDL not initialized - " << SDL_GetError() << std::endl;
            return;
        }
    }
}

Game::~Game(){
    if(storedstate != nullptr){
        delete storedstate;
    }
    while(!stateStack.empty()){
        stateStack.pop();
    }
    if(resources != nullptr){
        resources->ClearImages();
        resources->ClearMusics();
        resources->ClearSounds();
        resources->ClearFonts();
        delete resources;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    std::cout << "SDL Finalized" << std::endl;
}

SDL_Renderer* Game::GetRenderer(){
    return renderer;
}

State& Game::GetCurrentState(){
    return *(stateStack.top().get());
}

void Game::Push(State *state){
    storedstate = state;
}

void Game::Run(){
    if(storedstate){
        stateStack.emplace(storedstate);
        storedstate = nullptr;
        stateStack.top()->Start();
    }
    while((stateStack.top()->QuitRequested() == false) && (!(stateStack.empty()))){
        if(stateStack.top()->PopRequested()){
            stateStack.pop();
            resources->ClearImages();
            resources->ClearMusics();
            resources->ClearSounds();
            resources->ClearFonts();
            stateStack.top()->Resume();
        }
        if(storedstate){
            stateStack.top()->Pause();
            stateStack.emplace(storedstate);
            storedstate = nullptr;
            stateStack.top()->Start();
        }
        CalculateDeltaTime();
        InputManager::GetInstance().Update();
        stateStack.top()->Update(GetDeltaTime());
        stateStack.top()->Render();
        SDL_RenderPresent(renderer);
        // SDL_Delay(11);
        // std::cout << (int)(1/dt) << std::endl;
    }
}

void Game::CalculateDeltaTime(){
    dt = SDL_GetTicks() - frameStart;
    frameStart = SDL_GetTicks();
    dt = dt / 1000.0;
}

float Game::GetDeltaTime(){
    return this->dt;
}

