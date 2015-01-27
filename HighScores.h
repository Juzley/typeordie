#ifndef __HIGH_SCORES_H__
#define __HIGH_SCORES_H__

#include <memory>
#include <vector>
#include <string>
#include <memory>
#include <boost/utility.hpp>

namespace typing
{
    struct HighScore
    {
        unsigned int score;
        unsigned int streak;
        std::string  name;
    };
    typedef std::shared_ptr<HighScore> HighScorePtr;

    class HighScores : boost::noncopyable
    {
    public:
        // Typedefs
        typedef std::vector<HighScorePtr> ScoreVec;
        typedef ScoreVec::const_iterator ScoreIterator;

        // Consts/Enums
        static const unsigned int NAME_LENGTH = 3;

        // Methods
        void AddHighScore(unsigned int score, const std::string& name, unsigned int streak, bool loading);
        void Load();
        void Save() const;

        bool IsHighScore(unsigned int score) const
        {
            // m_scores is sorted in descending order of score, so
            // if this score is higher than the last score, it is
            // a high score.
            if (!m_scores.empty())
            {
                return (score > m_scores[m_scores.size() - 1]->score);
            }

            return true;
        }

        unsigned int GetHighScoreCount() const
        {
            return static_cast<unsigned int>(m_scores.size());
        }

        ScoreIterator begin() const
        {
            return m_scores.begin();
        }

        ScoreIterator end() const
        {
            return m_scores.end();
        }

        // Singleton Implementation
        static HighScores& GetHighScores();

    private:
        // Ctors/Dtors
        HighScores()
            : m_loaded(false)
        {
        }

        // Consts
        static const std::string  SCORE_FILE;
        static const unsigned int NUM_SCORES   = 10;

        // Members
        ScoreVec m_scores;
        bool     m_loaded;

        // Singleton Implementation
        static std::auto_ptr<HighScores> m_singleton;
    };
    #define SCORES HighScores::GetHighScores()
}

#endif // __HIGH_SCORES_H__
