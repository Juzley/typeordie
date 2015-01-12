#include <stdio.h>
#include <algorithm>
#include "HighScores.h"
#include "Exceptions.h"

namespace typing
{
    const std::string HighScores::SCORE_FILE("scores.dat");

    std::auto_ptr<HighScores> HighScores::m_singleton(new HighScores);
    HighScores& HighScores::GetHighScores()
    {
        return *(m_singleton.get());
    }

    class ScoreSort
    {
    public:
        bool operator()(const HighScorePtr& a, const HighScorePtr& b) const
        {
            return (a->score > b->score);
        }
    };

    void HighScores::AddHighScore(unsigned int score, const std::string& name, unsigned int streak, bool loading)
    {
        HighScorePtr scr(new HighScore());
        scr->score  = score;
        scr->name   = name;
        scr->streak = streak;

        m_scores.push_back(scr);

        // If we are loading the high scores from the score file, we don't want to sort and resize,
        // as we'll do this at the end, and we definitely don't want to save!
        if (!loading)
        {
            std::sort(m_scores.begin(), m_scores.end(), ScoreSort());
            m_scores.resize(NUM_SCORES);

            Save();
        }
    }

    void HighScores::Load()
    {
        if (!m_loaded)
        {
            unsigned int scores  = 0;

            FILE *file = fopen(SCORE_FILE.c_str(), "rb");
            if (file)
            {
                size_t       read    = 0;
                unsigned int score   = 0;
                unsigned int streak  = 0;
                size_t       nameLen = 0;
                char         c       = '\0';
                std::string  name;

                while (scores < NUM_SCORES)
                {
                    read = fread(&nameLen, sizeof(size_t), 1, file);
                    if (1 != read)
                    {
                        break;
                    }

                    name.clear();
                    for (size_t i = 0; i < nameLen; ++i)
                    {
                        read = fread(&c, sizeof(char), 1, file);
                        if (1 != read)
                        {
                            fclose(file);
                            throw FileCorruptException(SCORE_FILE);
                        }

                        name += c;
                    }

                    read = fread(&score, sizeof(unsigned int), 1, file);
                    if (1 != read)
                    {
                        fclose(file);
                        throw FileCorruptException(SCORE_FILE);
                    }

                    read = fread(&streak, sizeof(unsigned int), 1, file);
                    if (1 != read)
                    {
                        fclose(file);
                        throw FileCorruptException(SCORE_FILE);
                    }

                    AddHighScore(score, name, streak, true);
                    ++scores;
                }

                fclose(file);
            }

            // If we didn't read in enough scores, fill in the score table.
            for (unsigned int i = scores; i < NUM_SCORES; ++i)
            {
                AddHighScore(i * 100, "Juz", 5, true);
            }

            std::sort(m_scores.begin(), m_scores.end(), ScoreSort());
            m_scores.resize(NUM_SCORES);

            m_loaded = true;
        }
    }

    void HighScores::Save() const
    {
        FILE *file = fopen(SCORE_FILE.c_str(), "wb");
        if (!file)
        {
            throw FileNotFoundException(SCORE_FILE);
        }

        for (ScoreIterator iter = m_scores.begin(); iter != m_scores.end(); ++iter)
        {
            const size_t len = (*iter)->name.length();
            size_t written = fwrite(&len, sizeof(size_t), 1, file);
            if (1 != written)
            {
                fclose(file);
                throw FileWriteException(SCORE_FILE);
            }

            written = fwrite((*iter)->name.data(), sizeof(char), len, file);
            if (len != written)
            {
                fclose(file);
                throw FileWriteException(SCORE_FILE);
            }

            written = fwrite(&(*iter)->score, sizeof(unsigned int), 1, file);
            if (1 != written)
            {
                fclose(file);
                throw FileWriteException(SCORE_FILE);
            }

            written = fwrite(&(*iter)->streak, sizeof(unsigned int), 1, file);
            if (1 != written)
            {
                fclose(file);
                throw FileWriteException(SCORE_FILE);
            }
        }

        fclose(file);
    }
}

