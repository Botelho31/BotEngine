#include "../include/Game.h"
#include "../include/StageState.h"

int main(int argc,const char * argv[]){
    std::cout << argc << argv << "\n" << std::endl;
    Game game = Game::GetInstance();
    // game.Push(new StageState());
    // game.Run();
    return 0;
}