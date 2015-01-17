#include <memory>
#include <boost/random.hpp>

#ifndef __RANDOM_H__
#define __RANDOM_H__

namespace typing
{
    class Random
    {
    public:
        void Seed (unsigned int seed)
        {
            m_rng.seed(static_cast<boost::uint32_t>(seed));
        }

        int Range (int min, int max)
        {
            if (min == max) {
                return (max);
            } else {

                boost::uniform_int<> range(min, max);
                boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(m_rng, range);
                return die();
            }
        }

        unsigned int Range (unsigned int min, unsigned int max)
        {
            return Range(static_cast<int>(min), static_cast<int>(max));
        }

        float Range (float min, float max)
        {
            return (static_cast<float>(Range(static_cast<double>(min), static_cast<double>(max))));
        }

        double Range (double min, double max)
        {
            if (min == max) {
                return (max);
            } else {
                boost::uniform_real<> range(min, max);
                boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(m_rng, range);
                return die();
            }
        }

        // Singleton implementation
        static Random& GetRandom();
    private:
        boost::mt19937 m_rng;

        // Singleton implementation
        static std::auto_ptr<Random> m_singleton;
    };

    #define RAND Random::GetRandom()
}

#endif /* __RANDOM_H__ */
