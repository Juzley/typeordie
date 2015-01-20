#include <string>
#include <math.h>
#include <SDL/SDL.h>
#include "MenuNewHighScore.h"
#include "MenuMain.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "App.h"
#include "HighScores.h"
#include "Game.h"
#include "Utils.h"

namespace typing
{
    const std::string NewHighScoreMenu::MENU_NAME("NewHighScoreMenu");
    const std::string NewHighScoreMenu::FONT("fonts/menufont.fnt");
    const std::string NewHighScoreMenu::BACKGROUND("textures/menu/background.tga");
    const std::string NewHighScoreMenu::ERROR_SOUND("sounds/miss.wav");
    const float       NewHighScoreMenu::CURSOR_FLASH_SPEED = 0.3f;

    void NewHighScoreMenu::Init()
    {
        FONTS.Add(FONT);
        SOUNDS.Add(ERROR_SOUND);
        m_name.clear();
    }

    void NewHighScoreMenu::Draw()
    {
        const float BACKGROUND_MARGIN      = 100.0f;
        const float CONGRATS_HEIGHT        = 64.0f;
        const float CONGRATS_PAD           = 4.0f;
        const float SCORE_HEIGHT           = 48.0f;
        const float SCORE_PAD              = 10.0f;
        const float TEXT_HEIGHT            = 32.0f;
        const float TEXT_PAD               = 4.0f;
        const float ENTER_PAD              = 100.0f;
        const float NAME_HEIGHT            = 64.0f;
        const float NAME_PAD               = 10.0f;

        DrawTexturedRect(BACKGROUND, 0.0f, 0.0f, APP.GetScreenWidth(), APP.GetScreenHeight());

        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), BACKGROUND_MARGIN, 0.0f, APP.GetScreenWidth() - BACKGROUND_MARGIN * 2.0f,
                 APP.GetScreenHeight());

        const float x = APP.GetScreenWidth() / 2.0f;
        float y       = CONGRATS_PAD;
        FONTS.Print(FONT, x, y, CONGRATS_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    "Congratulations!");

        y += CONGRATS_HEIGHT + SCORE_PAD;
        FONTS.Print(FONT, x, y, SCORE_HEIGHT, ColourRGBA::Yellow(), Font::ALIGN_CENTER,
                    std::to_string(GAME.GetScore()));

        y += SCORE_HEIGHT + TEXT_PAD;
        FONTS.Print(FONT, x, y, TEXT_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
            "is a new high score!");

        y += ENTER_PAD + TEXT_PAD;
        FONTS.Print(FONT, x, y, TEXT_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    "Enter your name and");
        y += TEXT_HEIGHT + TEXT_PAD;
        FONTS.Print(FONT, x, y, TEXT_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER,
                    "press return to continue.");

        y += TEXT_HEIGHT + NAME_PAD;
        FONTS.Print(FONT, x, y, NAME_HEIGHT, ColourRGBA::Yellow(), Font::ALIGN_CENTER, m_name);

        if (m_name.length() < MAX_NAME_LENGTH && static_cast<int>(floor(APP.GetTime() / CURSOR_FLASH_SPEED)) % 2 == 0)
        {
            const float cursorX = FONTS.GetLineWidth(FONT, NAME_HEIGHT, m_name) / 2.0f + x;
            FONTS.Print(FONT, cursorX, y, NAME_HEIGHT, ColourRGBA::Yellow(), Font::ALIGN_LEFT, "_");
        }
    }

    const std::string NewHighScoreMenu::NextMenu() const
    {
        return MainMenu::MENU_NAME;
    }

    MenuScreen::NextAction NewHighScoreMenu::OnType(const SDL_keysym sym)
    {
        if (sym.sym == SDLK_BACKSPACE && !m_name.empty())
        {
            m_name.erase(m_name.length() - 1, 1);
            return ACTION_NONE;
        }

        if (sym.sym == SDLK_RETURN)
        {
            if (!m_name.empty())
            {
                SCORES.AddHighScore(GAME.GetScore(), m_name, GAME.GetMaxStreak(), false);
                return ACTION_NEXT_AND_POP;
            }
            else
            {
                SOUNDS.Play(ERROR_SOUND);
                return ACTION_NONE;
            }
        }

        // If the high 9 bits of the unicode are 0, this is an ascii char
        // and we can work with it.
        if (sym.unicode != 0 && (sym.unicode & 0xFF80) == 0) {
            if (m_name.length() < MAX_NAME_LENGTH)
            {
                m_name += sym.unicode & 0x7F;
            }
            else
            {
                SOUNDS.Play(ERROR_SOUND);
            }
        }

        return ACTION_NONE;
    }
}
