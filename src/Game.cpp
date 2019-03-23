#include "../include/Game.h"

Game* Game::instance;

Game& Game::GetInstance(){
    if(instance != nullptr){
        return *instance;
    }
    else{
        Game *game = new Game("BotEngine",1024,600);
        return *instance;
    }
}

Game::Game(std::string Title,int Width,int Height){
    if(instance != nullptr){
        std::cout << "Fatal Error: Two instances being created\n" << std::endl;
        return;
    }
    else{
        instance = this;
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0){
            std::cout << "Game instance Created\n" << std::endl;
            std::cout << "Number of Img Libraries Initialized: " << IMG_Init(IMG_INIT_JPG  |  IMG_INIT_PNG  |  IMG_INIT_TIF) << "\n" << std::endl;
            if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024) == 0){
                std::cout << "Mix_OpenAudio Initialized\n" << std::endl;
            }else{
                std::cout << "Fatal Error Loading OpenAudio: " << SDL_GetError() << "\n" << std::endl; 
            }
            std::cout << "Number of Mix Libraries Initialized: " << Mix_Init(MIX_INIT_MP3) << "\n" << std::endl;
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
            }else{
                std::cout << "Fatal Error Loading Renderer: " << SDL_GetError() << "\n" << std::endl; 
                std::cout << "Number of Drivers: " << SDL_GetNumRenderDrivers() << "\n" << std::endl;
                SDL_RendererInfo info;
                SDL_GetRendererInfo(renderer,&info);
                std::cout << "Driver in use: " << info.name << "\n" << std::endl;
            }
            state = new State();

        }else{
            std::cout << "Fatal Error: SDL not initialized - " << SDL_GetError() << std::endl;
            return;
        }
    }
}

Game::~Game(){
    IMG_Quit();
    Mix_Quit();
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "SDL Finalized" << std::endl;
}

SDL_Renderer* Game::GetRenderer(){
    return renderer;
}

State& Game::GetState(){
    return *state;
}

void Game::Run(){
    while(state->QuitRequested() == false){
        float dt = 0;
        state->Update(dt);
        state->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }
}

