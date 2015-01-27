#include "Award.h"
#include "FontManager.h"
#include "Game.h"

namespace typing
{
    const std::string Award::AWARD_FONT("fonts/hudfont.fnt");
    const float       Award::AWARD_RISE_TIME    = 0.2f;
    const float       Award::AWARD_FADE_TIME    = 1.0f;
    const float       Award::AWARD_DISPLAY_TIME = AWARD_RISE_TIME + AWARD_FADE_TIME;
    const float       Award::AWARD_FONT_HEIGHT  = 24.0f;
    const float       Award::AWARD_START_ALPHA  = 0.0f;
    const float       Award::AWARD_PEAK_ALPHA   = 1.0f;
    const float       Award::AWARD_END_ALPHA    = 0.0f;

    const std::string Award::AWARD_EXCELLENT_STRING("EXCELLENT");
    const ColourRGB   Award::AWARD_EXCELLENT_COLOUR(0.0f, 1.0f, 0.0f);

    const std::string Award::AWARD_GOOD_STRING("GOOD");
    const ColourRGB   Award::AWARD_GOOD_COLOUR(0.0f, 1.0f, 1.0f);

    const std::string Award::AWARD_OK_STRING("OK");
    const ColourRGB   Award::AWARD_OK_COLOUR(0.0f, 0.0f, 1.0f);

    const std::string Award::AWARD_POOR_STRING("POOR");
    const ColourRGB   Award::AWARD_POOR_COLOUR(1.0f, 0.0f, 1.0f);

    const std::string Award::AWARD_BAD_STRING("BAD");
    const ColourRGB   Award::AWARD_BAD_COLOUR(1.0f, 0.0f, 0.0f);

    const std::string Award::AWARD_EXTRALIFE_STRING("EXTRA LIFE!");
    const ColourRGB   Award::AWARD_EXTRALIFE_COLOUR(0.0f, 1.0f, 0.0f);

    const std::string Award::AWARD_SHORTEN_PHRASES_STRING("SHORTER PHRASES!");
    const ColourRGB   Award::AWARD_SHORTEN_PHRASES_COLOUR(0.0f, 1.0f, 0.0f);


    void Award::Init()
    {
        FONTS.Add(AWARD_FONT);
    }

    bool Award::Unlink()
    {
        if (GAME.GetTime() - m_startTime > AWARD_DISPLAY_TIME)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void Award::Draw()
    {
        float              y     = m_origin.GetY() - AWARD_FONT_HEIGHT / 2.0f;
        const ColourRGB   *col;
        const std::string *str;

        float lifeTime = GAME.GetTime() - m_startTime;
        float alpha = 0.0f;
        if (lifeTime < AWARD_RISE_TIME)
        {
            alpha = (lifeTime / AWARD_RISE_TIME) * (AWARD_PEAK_ALPHA - AWARD_START_ALPHA);
        }
        else if (lifeTime < AWARD_RISE_TIME + AWARD_FADE_TIME)
        {
            alpha = ((lifeTime - AWARD_RISE_TIME) / AWARD_FADE_TIME) * (AWARD_PEAK_ALPHA - AWARD_END_ALPHA);
            alpha = AWARD_PEAK_ALPHA - alpha;
        }

        switch (m_type)
        {
        case AWARD_EXCELLENT:
            col = &AWARD_EXCELLENT_COLOUR;
            str = &AWARD_EXCELLENT_STRING;
            break;

        case AWARD_GOOD:
            col = &AWARD_GOOD_COLOUR;
            str = &AWARD_GOOD_STRING;
            break;

        case AWARD_OK:
            col = &AWARD_OK_COLOUR;
            str = &AWARD_OK_STRING;
            break;

        case AWARD_POOR:
            col = &AWARD_POOR_COLOUR;
            str = &AWARD_POOR_STRING;
            break;

        case AWARD_BAD:
            col = &AWARD_BAD_COLOUR;
            str = &AWARD_BAD_STRING;
            break;

        case AWARD_EXTRALIFE:
            col = &AWARD_EXTRALIFE_COLOUR;
            str = &AWARD_EXTRALIFE_STRING;
            break;

        case AWARD_SHORTEN_PHRASES:
            col = &AWARD_SHORTEN_PHRASES_COLOUR;
            str = &AWARD_SHORTEN_PHRASES_STRING;
        }

        FONTS.Print(AWARD_FONT, m_origin.GetX(), y, AWARD_FONT_HEIGHT, ColourRGBA(*col, alpha), Font::ALIGN_CENTER, *str);
    }
}
