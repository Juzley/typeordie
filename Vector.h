#ifndef _VECTOR_H_
#define _VECTOR_H_

namespace juzutil
{
    //////////////////////////////////////////////////////////////////////////
    //
    // Forward Definitions
    //
    //////////////////////////////////////////////////////////////////////////
    class Vector3;

    //////////////////////////////////////////////////////////////////////////
    //
    // VECTOR2
    //
    //////////////////////////////////////////////////////////////////////////

    class Vector2
    {
    public:
        Vector2() { Clear(); }

        Vector2(const Vector2& vec) { *this = vec; }

        Vector2(const Vector3& vec);

        Vector2(float x, float y) { m_x = x; m_y = y; }

        void Clear() { m_x = m_y = 0.0f; }

        bool IsZeroVector() { return m_x == 0.0f && m_y == 0.0f; }

        float Size() const;
        float SquareSize() const { return m_x * m_x + m_y * m_y; }
        void Normalize() { *this /= Size(); }

        void Clamp(float minVal, float maxVal)
        {
            Clamp(minVal, minVal, maxVal, maxVal);
        }

        void Clamp(const Vector2& minVec, const Vector2& maxVec)
        {
            Clamp(minVec.m_x, minVec.m_y, maxVec.m_x, maxVec.m_y);
        }

        void Clamp(float minValX, float minValY, float maxValX, float maxValY)
        {
            if(m_x < minValX)
            {
                m_x = minValX;
            }
            if(m_x > maxValX)
            {
                m_x = maxValX;
            }
            if(m_y < minValY)
            {
                m_y = minValY;
            }
            if(m_y > maxValY)
            {
                m_y = minValY;
            }
        }

        void Round();

        bool Equals(const Vector2& vec) const { return m_x == vec.m_x && m_y == vec.m_y; }
        bool Equals(const Vector2& vec, float e) const;

        void Set(float x, float y) { m_x = x; m_y = y; }
        void SetX(float x) { m_x = x; }
        void SetY(float y) { m_y = y; }
        float GetX() const { return m_x; }
        float GetY() const { return m_y; }

        void operator=(const Vector2& vec)
        {
            m_x = vec.m_x; m_y = vec.m_y;
        }

        bool operator==(const Vector2& vec) const
        {
            return Equals(vec);
        }

        bool operator!=(const Vector2& vec) const
        {
            return !Equals(vec);
        }

        const Vector2 operator+(const Vector2& vec) const
        {
            return Vector2(m_x + vec.m_x, m_y + vec.m_y);
        }

        void operator+=(const Vector2& vec)
        {
            m_x += vec.m_x; m_y += vec.m_y;
        }

        const Vector2 operator-() const
        {
            return Vector2(-m_x, -m_y);
        }

        const Vector2 operator-(const Vector2& vec) const
        {
           return Vector2(m_x - vec.m_x, m_y - vec.m_y);
        }

        void operator-=(const Vector2& vec)
        {
            m_x -= vec.m_x; m_y -= vec.m_y;
        }

        float operator*(const Vector2& vec) const
        {
            return m_x * vec.m_x + m_y * vec.m_y;
        }

        const Vector2 operator*(float scale) const
        {
            return Vector2(m_x * scale, m_y * scale);
        }

        friend const Vector2 operator*(float scale, const Vector2& vec);

        void operator*=(float scale)
        {
            m_x *= scale; m_y *= scale;
        }

        const Vector2 operator/(float divide) const
        {
            return Vector2(m_x / divide, m_y / divide);
        }

        void operator/=(float divide)
        {
            m_x /= divide; m_y /= divide;
        }

        float operator[](int i) const
        {
            if(i == 1)
            {
                return m_y;
            }
            return m_x;
        }

        float& operator[](int i)
        {
            if(i == 1)
            {
                return m_y;
            }
            return m_x;
        }

        void ToArray(float* array) const
        {
            array[0] = m_x;
            array[1] = m_y;
        }

        static const int VEC2_X = 0;
        static const int VEC2_Y = 1;

    private:
        float m_x;
        float m_y;
    };

    //////////////////////////////////////////////////////////////////////////
    //
    // VECTOR3
    //
    //////////////////////////////////////////////////////////////////////////

    class Vector3
    {
    public:
        Vector3() { Clear(); }

        Vector3(const Vector2& vec) { m_x = vec.GetX(), m_y = vec.GetY(); m_z = 0.0f; }

        Vector3(const Vector3& vec) { *this = vec; }

        Vector3(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }

        void Clear() { m_x = m_y = m_z = 0.0f; }

        bool IsZeroVector() { return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f; }

        float Size() const;
        float SquareSize() const { return m_x * m_x + m_y * m_y + m_z * m_z; }
        void Normalize() { *this /= Size(); }

        const Vector3 GetMajorAxis() const;

        void Clamp(float minVal, float maxVal)
        {
            Clamp(minVal, minVal, minVal, maxVal, maxVal, maxVal);
        }

        void Clamp(const Vector3& minVec, const Vector3& maxVec)
        {
            Clamp(minVec.m_x, minVec.m_y, minVec.m_z, maxVec.m_x, maxVec.m_y, maxVec.m_z);
        }

        void Clamp(float minValX, float minValY, float minValZ, float maxValX, float maxValY, float maxValZ)
        {
            if(m_x < minValX)
            {
                m_x = minValX;
            }
            if(m_x > maxValX)
            {
                m_x = maxValX;
            }
            if(m_y < minValY)
            {
                m_y = minValY;
            }
            if(m_y > maxValY)
            {
                m_y = maxValY;
            }
            if(m_z < minValZ)
            {
                m_z = minValX;
            }
            if(m_z > maxValZ)
            {
                m_z = maxValZ;
            }
        }

        void Round();

        bool Equals(const Vector3& vec, float e) const;
        bool Equals(const Vector3& vec) const
        {
            return m_x == vec.m_x && m_y == vec.m_y && m_z == vec.m_z;
        }

        void Set(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
        void SetX(float x) { m_x = x; }
        void SetY(float y) { m_y = y; }
        void SetZ(float z) { m_z = z; }
        float GetX() const { return m_x; }
        float GetY() const { return m_y; }
        float GetZ() const { return m_z; }

        void operator=(const Vector3& vec)
        {
            m_x = vec.m_x; m_y = vec.m_y; m_z = vec.m_z;
        }

        bool operator==(const Vector3& vec) const
        {
            return Equals(vec);
        }

        bool operator!=(const Vector3& vec) const
        {
            return !Equals(vec);
        }

        const Vector3 operator+(const Vector3& vec) const
        {
            return Vector3(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
        }

        void operator+=(const Vector3& vec)
        {
            m_x += vec.m_x; m_y += vec.m_y; m_z += vec.m_z;
        }

        const Vector3 operator-() const
        {
            return Vector3(-m_x, -m_y, -m_z);
        }

        const Vector3 operator-(const Vector3& vec) const
        {
            return Vector3(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z);
        }

        void operator-=(const Vector3& vec)
        {
            m_x -= vec.m_x; m_y -= vec.m_y; m_z -= vec.m_z;
        }

        float operator*(const Vector3& vec) const
        {
            return m_x * vec.m_x + m_y * vec.m_y + m_z * vec.m_z;
        }

        const Vector3 operator%(const Vector3& vec) const
        {
            return Vector3(m_y * vec.m_z - m_z * vec.m_y,
                           m_z * vec.m_x - m_x * vec.m_z,
                           m_x * vec.m_y - m_y * vec.m_x);
        }

        const Vector3 operator*(float scale) const
        {
            return Vector3(m_x * scale, m_y * scale, m_z * scale);
        }

        friend const Vector3 operator*(float scale, const Vector3& vec);

        void operator*=(float scale)
        {
            m_x *= scale; m_y *= scale; m_z *= scale;
        }

        const Vector3 operator/(float divide) const
        {
            return Vector3(m_x / divide, m_y / divide, m_z / divide);
        }

        void operator/=(float divide)
        {
            m_x /= divide; m_y /= divide; m_z /= divide;
        }

        float operator[](int i) const
        {
             if(i == 2)
             {
                  return m_z;
             }
             if(i == 1)
             {
                 return m_y;
             }
             return m_x;
        }

        float& operator[](int i)
        {
            if(i == 2)
            {
                return m_z;
            }
            if(i == 1)
            {
                 return m_y;
            }
            return m_x;
        }

        void ToArray(float* array) const
        {
            array[0] = m_x;
            array[1] = m_y;
            array[2] = m_z;
        }

        static const int VEC3_X = 0;
        static const int VEC3_Y = 1;
        static const int VEC3_Z = 2;

    private:
        float m_x;
        float m_y;
        float m_z;
    };

    //////////////////////////////////////////////////////////////////////////
    //
    // VECTOR4
    //
    //////////////////////////////////////////////////////////////////////////

    class Vector4
    {
    public:
        Vector4() { Clear(); }
        Vector4(const Vector4& vec) { *this = vec; }
        Vector4(float x, float y, float z, float w) { m_x = x; m_y = y; m_z = z; m_w = w; }

        void Clear() { m_x = m_y = m_z = m_w = 0.0f; }

        bool IsZeroVector() { return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f && m_w == 0.0f; }

        float Size() const;
        float SquareSize() const { return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w; }
        void Normalize() { *this /= Size(); }

        void Clamp(float minVal, float maxVal)
        {
            Clamp(minVal, minVal, minVal, minVal, maxVal, maxVal, maxVal, maxVal);
        }

        void Clamp(const Vector4& minVec, const Vector4& maxVec)
        {
            Clamp(minVec.m_x, minVec.m_y, minVec.m_z, minVec.m_w,
                maxVec.m_x, maxVec.m_y, maxVec.m_z, maxVec.m_w);
        }

        void Clamp(float minValX, float minValY, float minValZ, float minValW,
                float maxValX, float maxValY, float maxValZ, float maxValW)
        {
            if(m_x < minValX)
            {
                m_x = minValX;
            }
            if(m_x > maxValX)
            {
                m_x = maxValX;
            }
            if(m_y < minValY)
            {
                m_y = minValY;
            }
            if(m_y > maxValY)
            {
                m_y = maxValY;
            }
            if(m_z < minValZ)
            {
                m_z = minValX;
            }
            if(m_z > maxValZ)
            {
                m_z = maxValZ;
            }
            if(m_w < minValW)
            {
                m_w = minValW;
            }
            if(m_w > maxValW)
            {
                m_w = maxValW;
            }
        }

        void Round();

        bool Equals(const Vector4& vec, float e) const;
        bool Equals(const Vector4& vec) const
        {
            return m_x == vec.m_x && m_y == vec.m_y && m_z == vec.m_z && m_w == vec.m_w;
        }

        void Set(float x, float y, float z, float w) { m_x = x; m_y = y; m_z = z; m_w = w; }
        void SetX(float x) { m_x = x; }
        void SetY(float y) { m_y = y; }
        void SetZ(float z) { m_z = z; }
        void SetW(float w) { m_w = w; }
        float GetX() const { return m_x; }
        float GetY() const { return m_y; }
        float GetZ() const { return m_z; }
        float GetW() const { return m_w; }

        void operator=(const Vector4& vec)
        {
            m_x = vec.m_x; m_y = vec.m_y; m_z = vec.m_z; m_w = vec.m_w;
        }

        bool operator==(const Vector4& vec) const
        {
            return Equals(vec);
        }

        bool operator!=(const Vector4& vec) const
        {
            return !Equals(vec);
        }

        const Vector4 operator+(const Vector4& vec) const
        {
            return Vector4(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z, m_w + vec.m_w);
        }

        void operator+=(const Vector4& vec)
        {
            m_x += vec.m_x; m_y += vec.m_y; m_z += vec.m_z; m_w += vec.m_w;
        }

        const Vector4 operator-() const
        {
             return Vector4(-m_x, -m_y, -m_z, -m_w);
        }

        const Vector4 operator-(const Vector4& vec) const
        {
            return Vector4(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z, m_w - vec.m_w);
        }

        void operator-=(const Vector4& vec)
        {
            m_x -= vec.m_x; m_y -= vec.m_y; m_z -= vec.m_z; m_w -= vec.m_w;
        }

        float operator*(const Vector4& vec) const
        {
            return m_x * vec.m_x + m_y * vec.m_y + m_z * vec.m_z + m_w * vec.m_w;
        }

        const Vector4 operator*(float scale) const
        {
            return Vector4(m_x * scale, m_y * scale, m_z * scale, m_w * scale);
        }

        friend const Vector4 operator*(float scale, const Vector4& vec);

        void operator*=(float scale)
        {
            m_x *= scale; m_y *= scale; m_z *= scale; m_w *= scale;
        }

        const Vector4 operator/(float divide) const
        {
            return Vector4(m_x / divide, m_y / divide, m_z / divide, m_w / divide);
        }

        void operator/=(float divide)
        {
             m_x /= divide; m_y /= divide; m_z /= divide; m_w /= divide;
        }

        float operator[](int i) const
        {
            switch(i)
            {
            case 3:
                return m_w;
            case 2:
                return m_z;
            case 1:
                return m_y;
            default:
                return m_x;
            }
        }

        float& operator[](int i)
        {
            switch(i)
            {
            case 3:
                return m_w;
            case 2:
                return m_z;
            case 1:
                return m_y;
            default:
                return m_x;
            }
        }

        void ToArray(float* array) const
        {
            array[0] = m_x;
            array[1] = m_y;
            array[2] = m_z;
            array[3] = m_w;
        }

        static const int VEC4_X = 0;
        static const int VEC4_Y = 1;
        static const int VEC4_Z = 2;
        static const int VEC4_W = 3;

    private:
        float m_x;
        float m_y;
        float m_z;
        float m_w;
    };
}

#endif // _VECTOR_H_
