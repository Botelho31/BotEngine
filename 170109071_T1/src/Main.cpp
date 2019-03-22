#include "../include/Game.h"

int main(int argc,const char * argv[]){
    std::cout << argc << argv << "\n" << std::endl;
    Game game = Game::GetInstance();
    game.Run();
    return 0;
}