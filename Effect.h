#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <memory>

namespace typing
{
    class Effect
    {
    public:
        virtual ~Effect()
        {
        }

        virtual void Draw()   = 0;
        virtual bool Unlink() = 0;

        virtual void Update()
        {
        }

        virtual void OnSpawn()
        {
        }
    };
    typedef std::shared_ptr<Effect> EffectPtr;
}

#endif // _EFFECT_H_
