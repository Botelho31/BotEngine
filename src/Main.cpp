#include "../include/Game.h"
#include "../include/StageState.h"
#include "../include/TitleState.h"

int main(int argc, char ** argv){
    std::cout << argc << argv << "\n" << std::endl;
    Game &game = Game::GetInstance();
    game.Push(new TitleState());
    game.Run();
    delete &game;
    return 0;
}