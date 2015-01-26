#include "Phrase.h"
#include "FontManager.h"
#include "Utils.h"
#include "Game.h"

namespace typing
{
    const std::string Phrase::PHRASE_FONT("fonts/menufont.fnt");
    const float       Phrase::PHRASE_HEIGHT             = 18.0f;
    const float       Phrase::PHRASE_BORDER_GAP         = 3.0f;
    const float       Phrase::PHRASE_BORDER_LINE_LENGTH = 3.0f;

    // Nudge the phrase up a bit so that it doesn't cover the entity
    const float       Phrase::PHRASE_Y_OFFSET           = 15.0f;

    void Phrase::Init()
    {
        FONTS.Add(PHRASE_FONT);
    }

    bool Phrase::OnType(char c, float time)
    {
        if (m_phraseIndex < m_phrase.length() && m_phrase[m_phraseIndex] == c)
        {
            if (m_phraseIndex++ == 0)
            {
                m_startTime = time;
            }

            m_lastCorrectTypeTime = time;
            return true;
        }

        return false;
    }

    // This assumes that the view has already been set up in ortho projection.
    void Phrase::Draw(const juzutil::Vector3& origin,
                      PhraseDrawOption        option)
    {
        const float height =
            PHRASE_HEIGHT * APP.GetOption<float>("text-scale");

        if (!m_phrase.empty()) {
            const juzutil::Vector2 coords = GAME.GetCam().PerspectiveProject(origin);

            std::string typed;
            std::string remaining;

            typed.assign(m_phrase, 0, m_phraseIndex);
            remaining.assign(m_phrase,
                             m_phraseIndex,
                             m_phrase.length() - m_phraseIndex);

            const float typedWidth = FONTS.GetLineWidth(
                                            PHRASE_FONT, height, typed);
            const float remainingWidth = FONTS.GetLineWidth(
                                            PHRASE_FONT, height, remaining);
            const float totalWidth = typedWidth + remainingWidth;

            float x = coords[0];
            float y = coords[1] - height / 2.0f - PHRASE_BORDER_GAP -
                                                            PHRASE_Y_OFFSET;
                
            glPushMatrix();
            glTranslatef(x, y, 0.0f);

            if (option == PHRASE_DRAW_BACKWARDS) {
                glScalef(-1.0f, 1.0f, 1.0f);
            }

            glTranslatef(-totalWidth / 2.0f,
                         -height / 2.0f - PHRASE_BORDER_GAP,
                         0.0f);

            // Draw the backing
            DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f),
                     -PHRASE_BORDER_GAP,
                     -PHRASE_BORDER_GAP,
                     totalWidth + PHRASE_BORDER_GAP * 2,
                     height + PHRASE_BORDER_GAP * 2);

            // Draw the text
            FONTS.Print(PHRASE_FONT,
                        0.0f, 0.0f, height,
                        ColourRGBA::Red(),
                        Font::ALIGN_LEFT,
                        typed);
            FONTS.Print(PHRASE_FONT,
                        typedWidth, 0.0f, height,
                        ColourRGBA::White(),
                        Font::ALIGN_LEFT,
                        remaining);

            // Draw the corners of the backing
            x = -PHRASE_BORDER_GAP;
            y = -PHRASE_BORDER_GAP;
            DrawLine(ColourRGBA::White(), x, y, x, y + PHRASE_BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x + PHRASE_BORDER_LINE_LENGTH, y);
            x += totalWidth + PHRASE_BORDER_GAP * 2;
            DrawLine(ColourRGBA::White(), x, y, x, y + PHRASE_BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x - PHRASE_BORDER_LINE_LENGTH, y);
            y += height + PHRASE_BORDER_GAP * 2;
            DrawLine(ColourRGBA::White(), x, y, x, y - PHRASE_BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x - PHRASE_BORDER_LINE_LENGTH, y);
            x -= totalWidth + PHRASE_BORDER_GAP * 2;
            DrawLine(ColourRGBA::White(), x, y, x, y - PHRASE_BORDER_LINE_LENGTH);
            DrawLine(ColourRGBA::White(), x, y, x + PHRASE_BORDER_LINE_LENGTH, y);

            glPopMatrix();
        }
    }
}
