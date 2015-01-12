#include <math.h>
#include "Vector.h"

namespace juzutil
{
    //////////////////////////////////////////////////////////////////////////
    //
    // VECTOR2
    //
    //////////////////////////////////////////////////////////////////////////

    Vector2::Vector2(const Vector3& vec)
    {
        m_x = vec.GetX(); m_y = vec.GetY();
    }

    float Vector2::Size() const
    {
        return sqrt(m_x * m_x + m_y * m_y);
    }

    bool Vector2::Equals(const Vector2& vec, float e) const
    {
        if(fabsf(m_x - vec.m_x) > e)
        {
            return false;
        }
        if(fabsf(m_y - vec.m_y) > e)
        {
            return false;
        }
        return true;
    }

    const Vector3 Vector3::GetMajorAxis() const
    {
        float x = fabsf(m_x);
        float y = fabsf(m_y);
        float z = fabsf(m_z);

        if (fabsf(x) >= fabsf(y)) {
            if (fabsf(x) >= fabsf(z)) {
                return Vector3(1.0f, 0.0f, 0.0f);
            } else {
                return Vector3(0.0f, 0.0f, 1.0f);
            }
        } else {
            if (fabsf(y) >= fabsf(z)) {
                return Vector3(0.0f, 1.0f, 0.0f);
            } else {
                return Vector3(0.0f, 0.0f, 1.0f);
            }
        }
    }

    void Vector2::Round()
    {
        m_x = floor(m_x + 0.5f);
        m_y = floor(m_y + 0.5f);
    }

    const Vector2 operator*(float scale, const Vector2& vec)
    {
        return Vector2(scale * vec.m_x, scale * vec.m_y);
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // VECTOR3
    //
    //////////////////////////////////////////////////////////////////////////

    float Vector3::Size() const
    {
        return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    }

    bool Vector3::Equals(const Vector3& vec, float e) const
    {
        if(fabsf(m_x - vec.m_x) > e)
        {
            return false;
        }
        if(fabsf(m_y - vec.m_y) > e)
        {
            return false;
        }
        if(fabsf(m_z - vec.m_z) > e)
        {
            return false;
        }
        return true;
    }

    void Vector3::Round()
    {
        m_x = floor(m_x + 0.5f);
        m_y = floor(m_y + 0.5f);
        m_z = floor(m_z + 0.5f);
    }

    const Vector3 operator*(float scale, const Vector3& vec)
    {
        return Vector3(scale * vec.m_x, scale * vec.m_y, scale * vec.m_z);
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // VECTOR3
    //
    //////////////////////////////////////////////////////////////////////////

    float Vector4::Size() const
    {
        return sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
    }

    bool Vector4::Equals(const Vector4& vec, float e) const
    {
        if(fabsf(m_x - vec.m_x) > e)
        {
            return false;
        }
        if(fabsf(m_y - vec.m_y) > e)
        {
            return false;
        }
        if(fabsf(m_z - vec.m_z) > e)
        {
            return false;
        }
        if(fabsf(m_w - vec.m_w) > e)
        {
            return false;
        }
        return true;
    }

    void Vector4::Round()
    {
        m_x = floor(m_x + 0.5f);
        m_y = floor(m_y + 0.5f);
        m_z = floor(m_z + 0.5f);
        m_w = floor(m_w + 0.5f);
    }

    const Vector4 operator*(float scale, const Vector4& vec)
    {
        return Vector4(scale * vec.m_x, scale * vec.m_y, scale * vec.m_z, scale * vec.m_w);
    }
}
