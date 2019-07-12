#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef VELHO_H
    #define VELHO_H
    #include "Physics.h"
    #include "Text.h"

    class Velho : public Component{
        public:
            enum velhoState{IDLE,CHATTING};
            Velho(GameObject& associated);
            ~Velho();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void AddChatBox();
            void RemoveChatBox();
        private:
            velhoState state;
            GameObject *caixadetexto;
            GameObject *linhacaixadetexto;
            GameObject *textObj;
            Text *text;
            std::vector<std::string> texts;
            std::vector<std::string> texts2;
            int currenttext;
    };

#endif