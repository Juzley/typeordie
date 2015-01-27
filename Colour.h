#ifndef _COLOUR_H_
#define _COLOUR_H_

namespace typing
{
    class ColourRGB
    {
    public:
        // Ctors/Dtors
        ColourRGB()
        {
            Clear();
        }

        ColourRGB(const ColourRGB& Col)
        {
            m_red = Col.m_red; m_green = Col.m_green; m_blue = Col.m_blue;
        }

        ColourRGB(float red, float green, float blue)
        {
            m_red = red; m_green = green; m_blue = blue;
        }


        // Methods
        void Clear()
        {
            m_red = m_green = m_blue = 1.0f;
        }

        void Set(float red, float green, float blue)
        {
            m_red = red; m_green = green; m_blue = blue;
        }

        void SetRed(float red)
        {
            m_red = red;
        }

        void SetGreen(float green)
        {
            m_green = green;
        }

        void SetBlue (float blue)
        {
            m_blue = blue;
        }

        float GetRed() const
        {
            return m_red;
        }

        float GetGreen() const
        {
            return m_green;
        }

        float GetBlue() const
        {
            return m_blue;
        }

        void operator = (const ColourRGB& Col)
        {
            m_red = Col.m_red; m_green = Col.m_green; m_blue = Col.m_blue;
        }

        float& operator [] (int i)
        {
            switch(i)
            {
            case COLOUR_BLUE:
                return m_blue;
            case COLOUR_GREEN:
                return m_green;
            default:
            case COLOUR_RED:
                return m_red;
            }
        }

        // Consts/Enums
        static const int COLOUR_RED   = 0;
        static const int COLOUR_GREEN = 1;
        static const int COLOUR_BLUE  = 2;

        static const ColourRGB White() { return ColourRGB(1.0f, 1.0f, 1.0f); }
        static const ColourRGB Black() { return ColourRGB(0.0f, 0.0f, 0.0f); }
        static const ColourRGB Red()   { return ColourRGB(1.0f, 0.0f, 0.0f); }
        static const ColourRGB Green() { return ColourRGB(0.0f, 1.0f, 0.0f); }
        static const ColourRGB Blue()  { return ColourRGB(0.0f, 0.0f, 1.0f); }

    private:
        float m_red;
        float m_green;
        float m_blue;
    };

	class ColourRGBA
	{
	public:
        // Ctors/Dtors
		ColourRGBA()
        {
            Clear();
        }

		ColourRGBA(const ColourRGBA& Col)
        {
            m_red = Col.m_red; m_green = Col.m_green; m_blue = Col.m_blue; m_alpha = Col.m_alpha;
        }

		ColourRGBA(float red, float green, float blue, float alpha)
        {
            m_red = red; m_green = green; m_blue = blue; m_alpha = alpha;
        }

        ColourRGBA(const ColourRGB& Col, float alpha)
        {
            m_red = Col.GetRed(); m_green = Col.GetGreen(); m_blue = Col.GetBlue(); m_alpha = alpha;
        }


        // Methods
		void Clear()
        {
            m_red = m_green = m_blue = m_alpha = 1.0f;
        }
		
        void Set(float red, float green, float blue, float alpha)
        {
            m_red = red; m_blue = blue; m_green = green; m_alpha = alpha;
        }

		void SetRed(float red)
        {
            m_red = red;
        }

		void SetGreen(float green)
        {
            m_green = green;
        }
		
        void SetBlue (float blue)
        {
            m_blue = blue;
        }

		void SetAlpha (float alpha)
        {
            m_alpha = alpha;
        }

		float GetRed() const
        {
            return m_red;
        }

		float GetGreen() const
        {
            return m_green;
        }

		float GetBlue() const
        {
            return m_blue;
        }

		float GetAlpha() const
        {
            return m_alpha;
        }

        const ColourRGB ToRGB() const
        {
            return ColourRGB(m_red, m_green, m_blue);
        }

		void operator = (const ColourRGBA& Col)
        {
            m_red = Col.m_red; m_green = Col.m_green; m_blue = Col.m_blue; m_alpha = Col.m_alpha;
        }

		float& operator [] (int i)
		{
			switch(i)
			{
			case COLOUR_ALPHA:
				return m_alpha;
			case COLOUR_BLUE:
				return m_blue;
			case COLOUR_GREEN:
				return m_green;
			default:
			case COLOUR_RED:
				return m_red;
			}
		}

        // Consts/Enums
		static const int COLOUR_RED   = 0;
		static const int COLOUR_GREEN = 1;
		static const int COLOUR_BLUE  = 2;
		static const int COLOUR_ALPHA = 3;

        static const ColourRGBA White()  { return ColourRGBA(1.0f, 1.0f, 1.0f, 1.0f); }
        static const ColourRGBA Black()  { return ColourRGBA(0.0f, 0.0f, 0.0f, 1.0f); }
        static const ColourRGBA Red()    { return ColourRGBA(1.0f, 0.0f, 0.0f, 1.0f); }
        static const ColourRGBA Green()  { return ColourRGBA(0.0f, 1.0f, 0.0f, 1.0f); }
        static const ColourRGBA Blue()   { return ColourRGBA(0.0f, 0.0f, 1.0f, 1.0f); }
        static const ColourRGBA Yellow() { return ColourRGBA(1.0f, 1.0f, 0.0f, 1.0f); }

	private:
		float m_red;
		float m_green;
		float m_blue;
		float m_alpha;
	};
}

#endif // _COLOUR_H_
