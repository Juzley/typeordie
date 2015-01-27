#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vector.h"

namespace juzutil
{
    class Matrix3;
    class Matrix4;

    #define MATRIX_INVERSE_EPSILON		1e-14
    #define MATRIX_EPSILON				1e-6

    //////////////////////////////////////////////////////////////////////////
    //
    // MATRIX2
    //
    // Column Major
    // v1 v2
    // ^^ ^^
    // xx yx  [0][0] [1][0]
    // xy yy  [0][1] [1][1]
    //
    //////////////////////////////////////////////////////////////////////////

    class Matrix2
    {
    public:
        Matrix2() {}

        Matrix2(const Matrix2& mat)
        {
            *this = mat;
        }

        Matrix2(const Vector2& x, const Vector2& y)
        {
            Set(x, y);
        }

        Matrix2(float xx, float xy, float yx, float yy)
        {
            Set(xx, xy, yx, yy);
        }

        void Clear()
        {
            m_matrix[0].Clear();
            m_matrix[1].Clear();
        }

        void Set(const Vector2& x, const Vector2& y)
        {
            m_matrix[0] = x;
            m_matrix[1] = y;
        }

        void Set(float xx, float xy, float yx, float yy)
        {
            m_matrix[0][0] = xx; m_matrix[1][0] = yx;
            m_matrix[0][1] = xy; m_matrix[1][1] = yy;
        }

        void SetToIdentity()
        {
            m_matrix[0].Set(1.0f, 0.0f);
            m_matrix[1].Set(0.0f, 1.0f);
        }

        bool Equals(const Matrix2& mat) const
        {
            return m_matrix[0] == mat[0] && m_matrix[1] == mat[1];
        }

        bool Equals(const Matrix2& mat, float e) const
        {
            return m_matrix[0].Equals(mat[0], e) && m_matrix[1].Equals(mat[1], e);
        }

        float Trace() const
        {
            return m_matrix[0][0] + m_matrix[1][1];
        }

        float Determinant() const
        {
            return m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0];
        }

        bool Inverse(Matrix2& mat) const;

        const Matrix2 Transpose() const
        {
            return Matrix2(m_matrix[0][0], m_matrix[1][0], m_matrix[0][1], m_matrix[1][1]);
        }

        void ToColumnMajorArray(float* array) const
        {
            array[0] = m_matrix[0][0]; array[2] = m_matrix[1][0];
            array[1] = m_matrix[0][1]; array[3] = m_matrix[1][1];
        }

        void ToRowMajorArray(float* array) const
        {
            array[0] = m_matrix[0][0]; array[1] = m_matrix[1][0];
            array[2] = m_matrix[0][1]; array[3] = m_matrix[1][1];
        }

        const Matrix3 ToMatrix3() const;
        const Matrix4 ToMatrix4() const;

        Vector2& operator[](int i)
        {
            return m_matrix[i];
        }

        const Vector2& operator[](int i) const
        {
            return m_matrix[i];
        }

        bool operator==(const Matrix2& mat) const
        {
            return Equals(mat);
        }

        bool operator!=(const Matrix2& mat) const
        {
            return !Equals(mat);
        }

        void operator=(const Matrix2& mat)
        {
            m_matrix[0] = mat[0];
            m_matrix[1] = mat[1];
        }

        const Matrix2 operator+(const Matrix2& mat) const
        {
            return Matrix2(m_matrix[0] + mat[0], m_matrix[1] + mat[1]);
        }

        void operator+=(const Matrix2& mat)
        {
            m_matrix[0] += mat[0];
            m_matrix[1] += mat[1];
        }

        const Matrix2 operator-(const Matrix2& mat) const
        {
            return Matrix2(m_matrix[0] - mat[0], m_matrix[1] - mat[1]);
        }

        void operator-=(const Matrix2& mat)
        {
            m_matrix[0] -= mat[0];
            m_matrix[1] -= mat[1];
        }

        const Matrix2 operator*(float scale) const
        {
            return Matrix2(m_matrix[0] * scale, m_matrix[1] * scale);
        }

        void operator*=(float scale)
        {
            m_matrix[0] *= scale;
            m_matrix[1] *= scale;
        }

        const Matrix2 operator*(const Matrix2& mat) const
        {
            return Matrix2(m_matrix[0][0] * mat[0][0] + m_matrix[1][0] * mat[0][1],
                           m_matrix[0][1] * mat[0][0] + m_matrix[1][1] * mat[0][1],
                           m_matrix[0][0] * mat[1][0] + m_matrix[1][0] * mat[1][1],
                           m_matrix[0][1] * mat[1][0] + m_matrix[1][1] * mat[1][1]);
        }

        void operator*=(const Matrix2& mat)
        {
            float tx, ty;
            tx = m_matrix[0][0]; ty = m_matrix[0][1];
            m_matrix[0][0] = tx * mat[0][0] + ty * mat[1][0];
            m_matrix[0][1] = tx * mat[0][1] + ty * mat[1][1];
            tx = m_matrix[1][0]; ty = m_matrix[1][1];
            m_matrix[1][0] = tx * mat[0][0] + ty * mat[1][0];
            m_matrix[1][1] = tx * mat[0][1] + ty * mat[1][1];
        }

        const Vector2 operator*(const Vector2& Vec) const
        {
            return Vector2(m_matrix[0][0] * Vec[0] + m_matrix[1][0] * Vec[1],
                           m_matrix[0][1] * Vec[0] + m_matrix[1][1] * Vec[1]);
        }

    private:
        Vector2 m_matrix[2];
    };

    //////////////////////////////////////////////////////////////////////////
    //
    // MATRIX3
    //
    // Column Major
    // v1 v2 v3
    // ^^ ^^ ^^
    // xx yx zx  [0][0] [1][0] [2][0]
    // xy yy zy  [0][1] [1][1] [2][1]
    // xz yz zz  [0][2] [1][2] [2][2]
    //
    //////////////////////////////////////////////////////////////////////////

    class Matrix3
    {
    public:
        Matrix3() {}

        Matrix3(const Matrix3& mat)
        {
            *this = mat;
        }

        Matrix3(const Vector3& x, const Vector3& y, const Vector3& z)
        {
            Set(x, y, z);
        }

        Matrix3(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz)
        {
            Set(xx, xy, xz, yx, yy, yz, zx, zy, zz);
        }

        void Clear()
        {
            m_matrix[0].Clear();
            m_matrix[1].Clear();
            m_matrix[2].Clear();
        }

        void Set(const Vector3& x, const Vector3& y, const Vector3& z)
        {
            m_matrix[0] = x;
            m_matrix[1] = y;
            m_matrix[2] = z;
        }

        void Set(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz)
        {
            m_matrix[0][0] = xx; m_matrix[1][0] = yx; m_matrix[2][0] = zx;
            m_matrix[0][1] = xy; m_matrix[1][1] = yy; m_matrix[2][1] = zy;
            m_matrix[0][2] = xz; m_matrix[1][2] = yz; m_matrix[2][2] = zz;
        }

        void SetToIdentity()
        {
            m_matrix[0].Set(1.0f, 0.0f, 0.0f);
            m_matrix[1].Set(0.0f, 1.0f, 0.0f);
            m_matrix[2].Set(0.0f, 0.0f, 1.0f);
        }

        bool Equals(const Matrix3& mat) const
        {
            return m_matrix[0] == mat[0] && m_matrix[1] == mat[1] && m_matrix[2] == mat[2];
        }

        bool Equals(const Matrix3& mat ,float e) const
        {
            return m_matrix[0].Equals(mat[0], e) && m_matrix[1].Equals(mat[1], e) && m_matrix[2].Equals(mat[2], e);
        }

        float Trace() const
        {
            return m_matrix[0][0] + m_matrix[1][1] + m_matrix[2][2];
        }

        float Determinant() const
        {
            return    m_matrix[0][0] * (m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1])
                    - m_matrix[0][1] * (m_matrix[1][0] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][0])
                    + m_matrix[0][2] * (m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0]);
        }

        bool Inverse(Matrix3& mat) const;

        const Matrix3 Transpose() const
        {
             return Matrix3(m_matrix[0][0], m_matrix[1][0], m_matrix[2][0],
                            m_matrix[0][1], m_matrix[1][1], m_matrix[2][1],
                            m_matrix[0][2], m_matrix[1][2], m_matrix[2][2]);
        }

        void ToColumnMajorArray(float* array) const
        {
            array[0] = m_matrix[0][0]; array[3] = m_matrix[1][0]; array[6] = m_matrix[2][0];
            array[1] = m_matrix[0][1]; array[4] = m_matrix[1][1]; array[7] = m_matrix[2][1];
            array[2] = m_matrix[0][2]; array[5] = m_matrix[1][2]; array[8] = m_matrix[2][2];
        }

        void ToRowMajorArray(float* array) const
        {
            array[0] = m_matrix[0][0]; array[1] = m_matrix[1][0]; array[2] = m_matrix[2][0];
            array[3] = m_matrix[0][1]; array[4] = m_matrix[1][1]; array[5] = m_matrix[2][1];
            array[6] = m_matrix[0][2]; array[7] = m_matrix[1][2]; array[8] = m_matrix[2][2];
        }

        const Matrix4 ToMatrix4() const;

        Vector3& operator[](int i)
        {
             return m_matrix[i];
        }

        const Vector3& operator[](int i) const
        {
            return m_matrix[i];
        }

        bool operator==(const Matrix3& mat) const
        {
            return Equals(mat);
        }

        bool operator!=(const Matrix3& mat) const
        {
            return !Equals(mat);
        }

        void operator=(const Matrix3& mat)
        {
            m_matrix[0] = mat[0];
            m_matrix[1] = mat[1];
            m_matrix[2] = mat[2];
        }

        const Matrix3 operator+(const Matrix3& mat) const
        {
            return Matrix3(m_matrix[0] + mat[0], m_matrix[1] + mat[1], m_matrix[2] + mat[2]);
        }

        void operator+=(const Matrix3& mat)
        {
            m_matrix[0] += mat[0];
            m_matrix[1] += mat[1];
            m_matrix[2] += mat[2];
        }

        const Matrix3 operator-(const Matrix3& mat) const
        {
            return Matrix3(m_matrix[0] - mat[0], m_matrix[1] - mat[1], m_matrix[2] - mat[2]);
        }

        void operator-=(const Matrix3& mat)
        {
            m_matrix[0] -= mat[0];
            m_matrix[1] -= mat[1];
            m_matrix[2] -= mat[2];
        }

        const Matrix3 operator*(float scale) const
        {
            return Matrix3(m_matrix[0] * scale, m_matrix[1] * scale, m_matrix[2] * scale);
        }

        void operator*=(float scale)
        {
            m_matrix[0] *= scale;
            m_matrix[1] *= scale;
            m_matrix[2] *= scale;
        }

        const Matrix3 operator*(const Matrix3& mat) const
        {
            Matrix3 result;
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    result[j][i] = m_matrix[0][i] * mat[j][0] +
                                   m_matrix[1][i] * mat[j][1] +
                                   m_matrix[2][i] * mat[j][2];

                }
            }

            return result;
        }

        void operator*=(const Matrix3& mat)
        {
            *this = *this * mat;
        }

        const Vector3 operator*(const Vector3& Vec) const
        {
            return Vector3(m_matrix[0][0] * Vec[0] + m_matrix[1][0] * Vec[1] + m_matrix[2][0] * Vec[2],
                m_matrix[0][1] * Vec[0] + m_matrix[1][1] * Vec[1] + m_matrix[2][1] * Vec[2],
                m_matrix[0][2] * Vec[0] + m_matrix[1][2] * Vec[1] + m_matrix[2][2] * Vec[2]);
        }

    private:
        Vector3 m_matrix[3];
    };

    //////////////////////////////////////////////////////////////////////////
    //
    // MATRIX4
    //
    // Column Major
    // v1 v2 v3 v4
    // ^^ ^^ ^^ ^^
    // xx yx zx wx  [0][0] [1][0] [2][0] [3][0]
    // xy yy zy wy  [0][1] [1][1] [2][1] [3][1]
    // xz yz zz wz  [0][2] [1][2] [2][2] [3][2]
    // xw yw zw ww  [0][3] [1][3] [2][3] [3][3]
    //
    //////////////////////////////////////////////////////////////////////////

    class Matrix4
    {
    public:
        Matrix4() {}

        Matrix4(const Matrix4& mat)
        {
            *this = mat;
        }

        Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w)
        {
            m_matrix[0] = x;
            m_matrix[1] = y;
            m_matrix[2] = z;
            m_matrix[3] = w;
        }

        Matrix4(float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw,
                float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww)
        {
            Set(xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wy, wz, ww);
        }

        Matrix4(const Matrix3& rotMat, const Vector3& transVec)
        {
            SetToHomogeneous(rotMat, transVec);
        }

        void Clear()
        {
            m_matrix[0].Clear();
            m_matrix[1].Clear();
            m_matrix[2].Clear();
            m_matrix[3].Clear();
        }

        void Set(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w)
        {
            m_matrix[0] = x;
            m_matrix[1] = y;
            m_matrix[2] = z;
            m_matrix[3] = w;
        }

        void Set(float xx, float xy, float xz, float xw, float yx, float yy, float yz, float yw,
            float zx, float zy, float zz, float zw, float wx, float wy, float wz, float ww)
        {
            m_matrix[0][0] = xx; m_matrix[1][0] = yx; m_matrix[2][0] = zx; m_matrix[3][0] = wx;
            m_matrix[0][1] = xy; m_matrix[1][1] = yy; m_matrix[2][1] = zy; m_matrix[3][1] = wy;
            m_matrix[0][2] = xz; m_matrix[1][2] = yz; m_matrix[2][2] = zz; m_matrix[3][2] = wz;
            m_matrix[0][3] = xw; m_matrix[1][3] = yw; m_matrix[2][3] = zw; m_matrix[3][3] = ww;
        }

        void SetToHomogeneous(const Matrix3& rotMat, const Vector3& transVec)
        {
            *this = rotMat.ToMatrix4();
            m_matrix[3][0] = transVec[0];
            m_matrix[3][1] = transVec[1];
            m_matrix[3][2] = transVec[2];
        }

        const Matrix3 RotMatFromHomogeneous() const
        {
            return Matrix3(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2],
                           m_matrix[1][0], m_matrix[1][1], m_matrix[1][2],
                           m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);
        }

        const Vector3 TransVecFromHomogeneous() const
        {
            return Vector3(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
        }

        void SetToIdentity()
        {
            m_matrix[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
            m_matrix[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
            m_matrix[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
            m_matrix[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
        }

        bool Equals(const Matrix4& mat) const
        {
            return m_matrix[0] == mat[0] && m_matrix[1] == mat[1] && m_matrix[2] == mat[2] && m_matrix[3] == mat[3];
        }

        bool Equals(const Matrix4& mat, float e) const
        {
            return m_matrix[0].Equals(mat[0], e) && m_matrix[1].Equals(mat[1], e) &&
                   m_matrix[2].Equals(mat[2], e) && m_matrix[3].Equals(mat[3], e);
        }

        float Trace() const
        {
            return m_matrix[0][0] + m_matrix[1][1] + m_matrix[2][2] + m_matrix[3][3];
        }

        float Determinant() const
        {
            // TODO meh
            return 0.0f;
        }

        bool Inverse(Matrix4& mat) const;

        const Matrix4 Transpose() const
        {
            return Matrix4(m_matrix[0][0], m_matrix[1][0], m_matrix[2][0], m_matrix[3][0],
                           m_matrix[0][1], m_matrix[1][1], m_matrix[2][1], m_matrix[3][1],
                           m_matrix[0][2], m_matrix[1][2], m_matrix[2][2], m_matrix[3][2],
                           m_matrix[0][3], m_matrix[1][3], m_matrix[2][3], m_matrix[3][3]);
        }

        void ToColumnMajorArray(float* array) const
        {
            array[0] = m_matrix[0][0]; array[4] = m_matrix[1][0]; array[8]  = m_matrix[2][0]; array[12] = m_matrix[3][0];
            array[1] = m_matrix[0][1]; array[5] = m_matrix[1][1]; array[9]  = m_matrix[2][1]; array[13] = m_matrix[3][1];
            array[2] = m_matrix[0][2]; array[6] = m_matrix[1][2]; array[10] = m_matrix[2][2]; array[14] = m_matrix[3][2];
            array[3] = m_matrix[0][3]; array[7] = m_matrix[1][3]; array[11] = m_matrix[2][3]; array[15] = m_matrix[3][3];
        }

        void ToRowMajorArray(float* array) const
        {
            array[0]  = m_matrix[0][0]; array[1]  = m_matrix[1][0]; array[2]  = m_matrix[2][0]; array[3]  = m_matrix[3][0];
            array[4]  = m_matrix[0][1]; array[5]  = m_matrix[1][1]; array[6]  = m_matrix[2][1]; array[7]  = m_matrix[3][1];
            array[8]  = m_matrix[0][2]; array[9]  = m_matrix[1][2]; array[10] = m_matrix[2][2]; array[11] = m_matrix[3][2];
            array[12] = m_matrix[0][3]; array[13] = m_matrix[1][3]; array[14] = m_matrix[2][3]; array[15] = m_matrix[3][3];
        }

        Vector4& operator[](int i)
        {
            return m_matrix[i];
        }

        const Vector4& operator[](int i) const
        {
            return m_matrix[i];
        }

        bool operator==(const Matrix4& mat) const
        {
            return Equals(mat);
        }

        bool operator!=(const Matrix4& mat) const
        {
            return !Equals(mat);
        }

        void operator=(const Matrix4& mat)
        {
            m_matrix[0] = mat[0];
            m_matrix[1] = mat[1];
            m_matrix[2] = mat[2];
            m_matrix[3] = mat[3];
        }

        const Matrix4 operator+(const Matrix4& mat) const
        {
            return Matrix4(m_matrix[0] + mat[0], m_matrix[1] + mat[1], m_matrix[2] + mat[2], m_matrix[3] + mat[3]);
        }

        void operator+=(const Matrix4& mat)
        {
            m_matrix[0] += mat[0];
            m_matrix[1] += mat[1];
            m_matrix[2] += mat[2];
            m_matrix[3] += mat[3];
        }

        const Matrix4 operator-(const Matrix4& mat) const
        {
            return Matrix4(m_matrix[0] - mat[0], m_matrix[1] - mat[1], m_matrix[2] - mat[2], m_matrix[3] - mat[3]);
        }

        void operator-=(const Matrix4& mat)
        {
            m_matrix[0] -= mat[0];
            m_matrix[1] -= mat[1];
            m_matrix[2] -= mat[2];
            m_matrix[3] -= mat[3];
        }

        const Matrix4 operator*(float scale) const
        {
            return Matrix4(m_matrix[0] * scale, m_matrix[1] * scale, m_matrix[2] * scale, m_matrix[3] * scale);
        }

        void operator*=(float scale)
        {
            m_matrix[0] *= scale;
            m_matrix[1] *= scale;
            m_matrix[2] *= scale;
            m_matrix[3] *= scale;
        }

        const Matrix4 operator*(const Matrix4& mat) const
        {
            Matrix4 result;
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    result[j][i] = m_matrix[0][i] * mat[j][0] +
                                   m_matrix[1][i] * mat[j][1] +
                                   m_matrix[2][i] * mat[j][2] +
                                   m_matrix[3][i] * mat[j][3];

                }
            }

            return result;
        }

        void operator*=(const Matrix4& mat)
        {
            *this = *this * mat;
        }

        const Vector4 operator*(const Vector4& Vec) const
        {
            return Vector4(m_matrix[0][0] * Vec[0] + m_matrix[1][0] * Vec[1] + m_matrix[2][0] * Vec[2] + m_matrix[3][0] * Vec[3],
                           m_matrix[0][1] * Vec[0] + m_matrix[1][1] * Vec[1] + m_matrix[2][1] * Vec[2] + m_matrix[3][1] * Vec[3],
                           m_matrix[0][2] * Vec[0] + m_matrix[1][2] * Vec[1] + m_matrix[2][2] * Vec[2] + m_matrix[3][2] * Vec[3],
                           m_matrix[0][3] * Vec[0] + m_matrix[1][3] * Vec[1] + m_matrix[2][3] * Vec[2] + m_matrix[3][3] * Vec[3]);
        }


    private:
        Vector4 m_matrix[4];
    };
}

#endif // _MATRIX_H_
