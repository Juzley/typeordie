#include <string>
#include <math.h>
#include "MenuHighScores.h"
#include "HighScores.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "App.h"
#include "Utils.h"

namespace typing
{
    const std::string HighScoresMenu::MENU_NAME("HighScoresMenu");
    const std::string HighScoresMenu::FONT("fonts/menufont.fnt");
    const std::string HighScoresMenu::BACKGROUND("textures/menu/background.tga");
    const float       HighScoresMenu::BACK_BUTTON_HEIGHT = 32.0f;
    const float       HighScoresMenu::BACK_BUTTON_PAD    = 2.0f;

    void HighScoresMenu::Init()
    {
        TEXTURES.Add(BACKGROUND);
        FONTS.Add(FONT);

        MenuItem::Init();

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(APP.GetScreenWidth() / 2.0f,
            APP.GetScreenHeight() - BACK_BUTTON_HEIGHT - BACK_BUTTON_PAD),
            "Back",  MENUITEM_BACK,  BACK_BUTTON_HEIGHT, true)));

        m_startTime = 0.0f;
    }

    MenuScreen::NextAction HighScoresMenu::Update()
    {
        if (m_startTime == 0.0f)
        {
            m_startTime = APP.GetTime();
        }

        return ACTION_NONE;
    }

    void HighScoresMenu::Draw()
    {
        const float BACKGROUND_MARGIN      = 50.0f;
        const float BACKGROUND_WIDTH       = APP.GetScreenWidth() - BACKGROUND_MARGIN * 2.0f;
        const float SECTION_SPACING        = 3.0f;
        const float TITLE_HEIGHT           = 64.0f;
        const float TITLE_BACKGROUND_PAD   = 4.0f;
        const float HEADING_HEIGHT         = 24.0f;
        const float HEADING_BACKGROUND_PAD = 2.0f;
        const float ENTRY_HEIGHT           = 32.0f;
        const float ENTRY_BACKGROUND_PAD   = 2.0f;
        const float ENTRY_TIME             = 0.1f;
        const float NAME_X                 = 55.0f;
        const float SCORE_X                = 150.0f;
        const float STREAK_X               = 650.0f;

        // Background
        DrawTexturedRect(BACKGROUND, 0.0f, 0.0f, APP.GetScreenWidth(), APP.GetScreenHeight());

        // Title "High Scores"
        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), BACKGROUND_MARGIN, 0.0f,
            BACKGROUND_WIDTH, TITLE_HEIGHT + TITLE_BACKGROUND_PAD * 2.0f);
        FONTS.Print(FONT, APP.GetScreenWidth() / 2.0f, TITLE_BACKGROUND_PAD, TITLE_HEIGHT, ColourRGBA::White(),
            Font::ALIGN_CENTER, "High Scores");

        float y = TITLE_HEIGHT + TITLE_BACKGROUND_PAD * 2.0f + SECTION_SPACING;

        // Score table headings
        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), BACKGROUND_MARGIN, y,
            BACKGROUND_WIDTH, HEADING_HEIGHT + HEADING_BACKGROUND_PAD * 2.0f);
        FONTS.Print(FONT, NAME_X, y + HEADING_BACKGROUND_PAD, HEADING_HEIGHT, ColourRGBA::White(),
            Font::ALIGN_LEFT, "Player");
        FONTS.Print(FONT, SCORE_X, y + HEADING_BACKGROUND_PAD, HEADING_HEIGHT, ColourRGBA::White(),
            Font::ALIGN_LEFT, "Score");
        FONTS.Print(FONT, STREAK_X, y + HEADING_BACKGROUND_PAD, HEADING_HEIGHT, ColourRGBA::White(),
            Font::ALIGN_LEFT, "Streak");

        y += HEADING_HEIGHT + HEADING_BACKGROUND_PAD * 2.0f + SECTION_SPACING;

        // Scores
        float entryEndY = APP.GetScreenHeight() - BACK_BUTTON_HEIGHT - BACK_BUTTON_PAD * 2.0f - SECTION_SPACING;

        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), BACKGROUND_MARGIN, y,
            BACKGROUND_WIDTH, entryEndY - y);

        float entryPad = (((entryEndY - y - ENTRY_BACKGROUND_PAD * 2.0f)) - ENTRY_HEIGHT * SCORES.GetHighScoreCount()) / (SCORES.GetHighScoreCount() - 1);
        y += ENTRY_BACKGROUND_PAD;

        const unsigned int numEntries = static_cast<unsigned int>(floor((APP.GetTime() - m_startTime) / ENTRY_TIME));

        float blue = 0;
        const float blueInc = 1.0f / SCORES.GetHighScoreCount();
        HighScores::ScoreIterator iter = SCORES.begin();
        for(unsigned int i = 0; i < numEntries && iter != SCORES.end(); ++i, ++iter)
        {
            ColourRGBA col(1.0f, 1.0f, blue, 1.0f);

            FONTS.Print(FONT, NAME_X, y, ENTRY_HEIGHT, col, Font::ALIGN_LEFT,
                        (*iter)->name);
            FONTS.Print(FONT, SCORE_X, y, ENTRY_HEIGHT, col, Font::ALIGN_LEFT,
                        std::to_string((*iter)->score));
            FONTS.Print(FONT, STREAK_X, y, ENTRY_HEIGHT, col, Font::ALIGN_LEFT,
                        std::to_string((*iter)->streak));

            y += ENTRY_HEIGHT + entryPad;
            blue += blueInc;
        }

        // Back button
        y = entryEndY + SECTION_SPACING;

        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), BACKGROUND_MARGIN, y,
            BACKGROUND_WIDTH, APP.GetScreenHeight() - y);
        MenuScreen::Draw();
    }

    MenuScreen::NextAction HighScoresMenu::OnMenuItemChoose(unsigned int)
    {
        return ACTION_PREV;
    }

    const std::string HighScoresMenu::NextMenu() const
    {
        return "";
    }
}
