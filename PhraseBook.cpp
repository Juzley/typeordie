#include <ctime>
#include <boost/format.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "App.h"
#include "PhraseBook.h"
#include "Exceptions.h"
#include "Random.h"
#include "FontManager.h"

namespace typing
{
    const std::string PhraseBook::PHRASE_FILE("phrases/phrases");

    void PhraseBook::Init()
    {
        FILE * phraseFile = fopen(PHRASE_FILE.c_str(), "r");
        if (phraseFile == NULL) {
            throw FileNotFoundException(PHRASE_FILE);
        }

        char buffer[MAX_PHRASE_LENGTH];
        while (fgets(buffer, MAX_PHRASE_LENGTH, phraseFile) != NULL) {
            char *c = strchr(buffer, '\r');
            if (c != NULL) {
                *c = '\0';
            }

            c = strchr(buffer, '\n');
            if (c != NULL) {
                *c = '\0';
            }

            AddPhrase(buffer);
        }

        fclose(phraseFile);
    }

    const std::string& PhraseBook::GetPhrase(PhraseLength len)
    {
        // We're returning a reference to avoid copying strings,
        // so we need this to return a reference to in case of error
        static const std::string defPhrase("default");

        // First pick a start character from the available ones.
        const char startChar = PickAvailChar();
        if (startChar == '\0') {
            return defPhrase;
        }

        PhraseVectorPtr phraseVec = GetPhraseVector(startChar, len);
        if (!phraseVec || phraseVec->empty()) {
            return defPhrase;
        }

        // Mark the start char unavailable so we don't end up with two phrases
        // in use which both have the same start char.
        MakeCharUnavail(startChar);

        // Select a random phrase from the vector of phrases.
        unsigned int i = RAND.Range(0, static_cast<int>(phraseVec->size() - 1));
        return *((*phraseVec)[i]);
    }

    const std::string PhraseBook::GetComboPhrase(unsigned int words,
                                                 PhraseLength length)
    {
        std::string phrase;

        for (unsigned int i = 0; i < words; i++) {
            const char c = (i == 0 ? PickAvailChar() : PickRandomChar());

            PhraseVectorPtr phraseVec;
            if (c != '\0' &&
                (phraseVec = GetPhraseVector(c, length)) &&
                !phraseVec->empty()) {
                
                if (!phrase.empty()) {
                    phrase += " ";
                }

                phrase += *((*phraseVec)[
                    RAND.Range(0, static_cast<int>(phraseVec->size() - 1))]);
            } else {
                return "default";
            }
        }

        MakeCharUnavail(phrase[0]);
        return (phrase);
    }

    void PhraseBook::MakeCharAvail(char c)
    {
        // We should never try to make a char available that we don't have
        // any phrases for, as a phrasebook user should only attempt to
        // make chars available for phrases it has been given from the
        // phrasebook.
        assert(m_phrases.find(c) != m_phrases.end());
        m_availChars.insert(c);
    }

    void PhraseBook::MakeAllCharsAvail()
    {
        for (PhraseMap::iterator iter = m_phrases.begin();
             iter != m_phrases.end();
             ++iter) {
            MakeCharAvail(iter->first);
        }
    }

    void PhraseBook::MakeCharUnavail(char c)
    {
        m_availChars.erase(c);
    }

    char PhraseBook::PickAvailChar()
    {
        if (m_availChars.empty()) {
            return '\0';
        } else {
            auto iter = m_availChars.begin();
            std::advance(iter,
                         RAND.Range(
                                0, static_cast<int>(m_availChars.size() - 1)));
            return *iter;
        }
    }

    char PhraseBook::PickRandomChar()
    {
        if (m_allChars.empty()) {
            return '\0';
        } else {
            auto iter = m_allChars.begin();
            std::advance(iter,
                         RAND.Range(
                                0, static_cast<int>(m_allChars.size() - 1)));
            return *iter;
        }
    }

    void PhraseBook::AddPhrase(const std::string& phrase)
    {
        const unsigned int len = static_cast<unsigned int>(phrase.length());
        if (len == 0)
        {
            return;
        }

        const char startChar = phrase[0];

        // Create the arrays for this character if we don't already have them
        if (m_phrases.find(startChar) == m_phrases.end())
        {
            m_phrases[startChar] = PhraseArrayPtr(new PhraseArray);
            (*m_phrases[startChar])[PL_SINGLE] =
                                        PhraseVectorPtr(new PhraseVector());
            (*m_phrases[startChar])[PL_SHORT] =
                                        PhraseVectorPtr(new PhraseVector());
            (*m_phrases[startChar])[PL_MEDIUM] =
                                        PhraseVectorPtr(new PhraseVector());
            (*m_phrases[startChar])[PL_LONG] =
                                        PhraseVectorPtr(new PhraseVector());
        }

        PhraseVectorPtr vec = GetPhraseVector(startChar, LengthToCategory(len));
        if (vec)
        {
            PhrasePtr p(new std::string(phrase));
            vec->push_back(p);

            MakeCharAvail(startChar);

            // Add the char to the list of all characters
            m_allChars.insert(startChar);
        }
    }

    PhraseBook::PhraseVectorPtr PhraseBook::GetPhraseVector(
                                                    char         startChar,
                                                    PhraseLength cat)
    {
        PhraseVectorPtr vec;

        // We should never get a phrase vector for a start char that
        // we don't have an array for.
        assert(m_phrases.find(startChar) != m_phrases.end());
        assert(cat >= PL_SINGLE && cat < PL_COUNT);

        vec = (*m_phrases[startChar])[cat];
        if (!vec) {
            APP.Log(App::LOG_DEBUG,
                    boost::str(boost::format(
                        "Couldn't find phrases for char %c category %u") %
                        startChar % cat));

            // If we didn't find a word of this length, try a shorter one.
            while (!vec && cat >= PL_SINGLE) {
                cat = static_cast<PhraseLength>(static_cast<int>(cat) - 1);
                vec = (*m_phrases[startChar])[cat];
            }
        }

        return vec;
    }

    PhraseBook::PhraseLength PhraseBook::LengthToCategory(unsigned int len)
    {
        if (len <= SINGLE_PHRASE_LENGTH) {
            return PL_SINGLE;
        } else if (len <= SHORT_PHRASE_LENGTH) {
            return PL_SHORT;
        } else if (len <= MEDIUM_PHRASE_LENGTH) {
            return PL_MEDIUM;
        } else {
            return PL_LONG;
        }
    }


#ifdef _DEBUG
    void PhraseBook::DrawChars(const std::string &font, float y, float height)
    {
        float x = 0.0f;
        for (PhraseMap::iterator iter = m_phrases.begin(); iter != m_phrases.end(); ++iter)
        {
            PhraseVectorPtr vec = (*iter->second)[PL_SINGLE];
            std::string str = *((*vec)[0]);

            char c[2];
            c[0] = str[0];
            c[1] = '\0';

            bool avail = (std::find(m_availChars.begin(), m_availChars.end(), c[0]) != m_availChars.end());
            
            FONTS.Print(font, x, y, height, avail ? ColourRGBA::White() : ColourRGBA::Red(), Font::ALIGN_LEFT, c);

            x += FONTS.GetLineWidth(font, 16.0f, c);
        }
    }
#endif
}

