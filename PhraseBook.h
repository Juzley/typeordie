#ifndef __PHRASE_BOOK_H__
#define __PHRASE_BOOK_H__

#include <vector>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/random.hpp>

namespace typing
{
    class PhraseBook
    {
    public:
        // Consts/Enums
        enum PhraseLength { PL_SINGLE, PL_TINY, PL_SHORT, PL_MEDIUM, PL_LONG, PL_HUGE, PL_COUNT };

        // Methods
        void               Init();
        const std::string& GetPhrase(PhraseLength length);
        void               MakeCharAvail(char c);
        void               MakeAllCharsAvail();

#ifdef _DEBUG
        void               DrawChars(const std::string &font, float y, float height);
#endif

    private:
        // Typedefs
        typedef boost::shared_ptr<std::string>          PhrasePtr;
        typedef std::vector<PhrasePtr>                  PhraseVector;
        typedef boost::shared_ptr<PhraseVector>         PhraseVectorPtr;
        typedef boost::array<PhraseVectorPtr, PL_COUNT> PhraseArray;
        typedef boost::shared_ptr<PhraseArray>          PhraseArrayPtr;
        typedef std::map<char, PhraseArrayPtr>          PhraseMap;

        // Consts/Enums
        static const unsigned int SINGLE_PHRASE_LENGTH = 1;
        static const unsigned int TINY_PHRASE_LENGTH   = 5;
        static const unsigned int SHORT_PHRASE_LENGTH  = 8;
        static const unsigned int MEDIUM_PHRASE_LENGTH = 30;
        static const unsigned int LONG_PHRASE_LENGTH   = 50;
        static const unsigned int MAX_PHRASE_LENGTH    = 128;
        static const std::string  PHRASE_FILE;

        // Methods
        PhraseVectorPtr GetPhraseVector(char startChar, PhraseLength cat);
        PhraseLength    LengthToCategory(unsigned int len);
        void            AddPhrase(const std::string& phrase);
        void            MakeCharUnavail(char c);
        char            PickAvailChar();

        // Members
        PhraseMap           m_phrases;
        std::vector<char>   m_availChars;
    };
}

#endif // __PHRASE_BOOK_H__
