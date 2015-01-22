#ifndef __MENU_NEW_HIGH_SCORE__
#define __MENU_NEW_HIGH_SCORE__

#include "MenuScreen.h"

namespace typing
{
    class NewHighScoreMenu : public MenuScreen
    {
    public:
        void              Init();
        void              Draw();
        NextAction        OnKeyDown(SDL_Keycode keycode);
        void              OnType(char c);
        const std::string NextMenu() const;

        static const std::string MENU_NAME;

    private:
        // Constants
        static const int         MAX_NAME_LENGTH = 3;
        static const std::string FONT;
        static const std::string BACKGROUND;
        static const std::string ERROR_SOUND;
        static const float       CURSOR_FLASH_SPEED;

        // Members
        std::string m_name;
    };
}

#endif // __MENU_NEW_HIGH_SCORE__
