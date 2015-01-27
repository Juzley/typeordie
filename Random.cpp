#include "Random.h"

namespace typing
{
    std::auto_ptr<Random> Random::m_singleton(new Random);
    Random& Random::GetRandom()
    {
        return *(m_singleton.get());
    }
}