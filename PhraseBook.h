#ifndef __PHRASE_BOOK_H__
#define __PHRASE_BOOK_H__

#include <set>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <array>

namespace typing
{
    class PhraseBook
    {
    public:
        // Consts/Enums
        enum PhraseLength {
            PL_SINGLE,
            PL_SHORT,
            PL_MEDIUM,
            PL_LONG,
            PL_COUNT };

        // Methods
        void               Init();
        const std::string& GetPhrase(PhraseLength length);
        const std::string  GetComboPhrase(unsigned int words,
                                          PhraseLength length);
        void               MakeCharAvail(char c);
        void               MakeAllCharsAvail();

#ifdef _DEBUG
        void               DrawChars(const std::string &font, float y, float height);
#endif

    private:
        // Typedefs
        typedef std::shared_ptr<std::string>          PhrasePtr;
        typedef std::vector<PhrasePtr>                PhraseVector;
        typedef std::shared_ptr<PhraseVector>         PhraseVectorPtr;
        typedef std::array<PhraseVectorPtr, PL_COUNT> PhraseArray;
        typedef std::shared_ptr<PhraseArray>          PhraseArrayPtr;
        typedef std::map<char, PhraseArrayPtr>        PhraseMap;

        // Consts/Enums
        static const unsigned int SINGLE_PHRASE_LENGTH = 1;
        static const unsigned int SHORT_PHRASE_LENGTH  = 6;
        static const unsigned int MEDIUM_PHRASE_LENGTH = 12;
        static const unsigned int MAX_PHRASE_LENGTH    = 128;
        static const std::string  PHRASE_FILE;

        // Methods
        PhraseVectorPtr GetPhraseVector(char startChar, PhraseLength cat);
        PhraseLength    LengthToCategory(unsigned int len);
        void            AddPhrase(const std::string& phrase);
        void            MakeCharUnavail(char c);
        char            PickAvailChar();
        char            PickRandomChar();

        // Members
        PhraseMap       m_phrases;
        std::set<char> m_availChars;
        std::set<char> m_allChars; 
    };
}

#endif // __PHRASE_BOOK_H__
