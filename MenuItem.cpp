#include "MenuItem.h"
#include "FontManager.h"
#include "Colour.h"
#include "Utils.h"

namespace typing
{
    const std::string MenuItem::FONT("fonts/menufont.fnt");
    const float       MenuItem::BORDER_GAP_X       = 2.0f;
    const float       MenuItem::BORDER_GAP_Y       = -2.0f;
    const float       MenuItem::BORDER_LINE_LENGTH = 5.0f;

    void MenuItem::Init()
    {
        FONTS.Add(FONT);
    }

    void MenuItem::Draw()
    {
        ColourRGBA col;
        if (IsSelected())
        {
            col = ColourRGBA::Red();

            float width = FONTS.GetLineWidth(FONT, m_height, m_text);
            float x     = m_origin.GetX() - BORDER_GAP_X - width / 2.0f;
            float y     = m_origin.GetY() - BORDER_GAP_Y;
            DrawLine(ColourRGBA::White(), x, y, x, y + BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x + BORDER_LINE_LENGTH, y);
            x += width + BORDER_GAP_X * 2;
            DrawLine(ColourRGBA::White(), x, y, x, y + BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x - BORDER_LINE_LENGTH, y);
            y += m_height + BORDER_GAP_Y * 2;
            DrawLine(ColourRGBA::White(), x, y, x, y - BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x - BORDER_LINE_LENGTH, y);
            x -= width + BORDER_GAP_X * 2;
            DrawLine(ColourRGBA::White(), x, y, x, y - BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x + BORDER_LINE_LENGTH, y);
        }
        else
        {
            col = ColourRGBA::White();
        }

        FONTS.Print(FONT, m_origin.GetX(), m_origin.GetY(), m_height, col, Font::ALIGN_CENTER, m_text);
    }
}
