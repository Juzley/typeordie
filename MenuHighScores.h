#ifndef __MENU_HIGH_SCORES_H__
#define __MENU_HIGH_SCORES_H__

#include <string>
#include "MenuScreen.h"

namespace typing
{
    class HighScoresMenu : public MenuScreen
    {
    public:
        void              Init();
        NextAction        Update();
        void              Draw();
        NextAction        OnMenuItemChoose(unsigned int id);
        const std::string NextMenu() const;

        static const std::string MENU_NAME;

    private:
        static const std::string  FONT;
        static const std::string  BACKGROUND;
        static const float        BACK_BUTTON_HEIGHT;
        static const float        BACK_BUTTON_PAD;
        static const unsigned int MENUITEM_BACK = 0;

        float m_startTime;
    };
}

#endif // __MENU_HIGH_SCORES_H__