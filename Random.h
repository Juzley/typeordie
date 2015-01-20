#include <memory>
#include <random>

#ifndef __RANDOM_H__
#define __RANDOM_H__

namespace typing
{
    class Random
    {
    public:
        void Seed (unsigned int seed)
        {
            m_rng.seed(seed);
        }

        int Range (int min, int max)
        {
            if (min == max) {
                return (max);
            } else {
                std::uniform_int_distribution<int> range(min, max);
                return range(m_rng);
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
                std::uniform_real_distribution<double> range(min, max);
                return range(m_rng);
            }
        }

        // Singleton implementation
        static Random& GetRandom();
    private:
        std::mt19937 m_rng;

        // Singleton implementation
        static std::auto_ptr<Random> m_singleton;
    };

    #define RAND Random::GetRandom()
}

#endif /* __RANDOM_H__ */
