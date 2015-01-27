#ifndef __PHRASE_H__
#define __PHRASE_H__

#include <string>
#include "Vector.h"

namespace typing
{
    class WorldToScreenCoords;

    class Phrase
    {
    public:
        static const std::string PHRASE_FONT;

        Phrase()
            : m_phraseIndex(0), m_startTime(0.0f), m_lastCorrectTypeTime(0.0f)
        {
        }

        Phrase(const std::string& phrase)
            : m_phrase(phrase), m_phraseIndex(0), m_startTime(0.0f),
            m_lastCorrectTypeTime(0.0f)
        {
        }

        static void Init();

        typedef enum {
            PHRASE_DRAW_DEFAULT,
            PHRASE_DRAW_BACKWARDS,
            PHRASE_DRAW_BLOCKED,
            PHRASE_DRAW_HIDDEN,
        } PhraseDrawOption;

        bool OnType(char c, float time);
        void Draw(const juzutil::Vector3& origin,
                  PhraseDrawOption        option = PHRASE_DRAW_DEFAULT);

        void Reset(const std::string& phrase)
        {
            m_phrase = phrase;
            m_phraseIndex = 0;
            m_startTime = 0.0f;
            m_lastCorrectTypeTime = 0.0f;
        }

        bool IsEmpty() const
        {
            return (m_phrase.empty());
        }

        char GetStartChar() const
        {
            if (m_phrase.empty())
            {
                return '\0';
            }

            return m_phrase[0];
        }

        unsigned int Length() const
        {
            return static_cast<unsigned int>(m_phrase.length());
        }

        bool Finished() const
        {
            return (m_phraseIndex >= m_phrase.length());
        }

        float GetTypingSpeed() const
        {
            if (m_phrase.empty())
            {
                return 0;
            }

            return (m_lastCorrectTypeTime - m_startTime) / static_cast<float>(m_phrase.length());
        }


    private:
        static const float       PHRASE_HEIGHT;
        static const float       PHRASE_BORDER_GAP;
        static const float       PHRASE_BORDER_LINE_LENGTH;
        static const float       PHRASE_Y_OFFSET;

        std::string  m_phrase;
        unsigned int m_phraseIndex;
        float        m_startTime;
        float        m_lastCorrectTypeTime;
    };
}

#endif // __PHRASE_H__
